#ifndef MINI_HTTP_MAINLOOP_H
#define MINI_HTTP_MAINLOOP_H

#include <arpa/inet.h>
#include <csignal>
#include <poll.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "route.h"
#include "set_socket.h"

static volatile sig_atomic_t keep_running = 1;
static void sig_handler(int) { keep_running = 0; }

static bool nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return flags >= 0 && fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0;
}
struct Connection {
    std::string input;
    Outgoing output;
};
// poll is available on both macOS and Linux.
inline int mainloop(const char *server_ip, int port, httprouter::node &routes)
{
    keep_running = 1;
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGPIPE, SIG_IGN);
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) { perror("socket"); return 1; }
    int reuse = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &address.sin_addr) != 1) {
        fprintf(stderr, "Invalid bind address: %s\n", server_ip);
        close(listener);
        return 1;
    }
    if (::bind(listener, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0 ||
        ::listen(listener, SOMAXCONN) < 0 || !nonblocking(listener)) {
        perror("bind/listen");
        close(listener);
        return 1;
    }
    printf("Listening on http://%s:%d\n", server_ip, port);
    fflush(stdout);
    std::vector<pollfd> clients{{listener, POLLIN, 0}};
    std::map<int, Connection> connections;
    int result = 0;
    while (keep_running) {
        int ready = poll(clients.data(), clients.size(), 250);
        if (ready < 0) {
            if (errno == EINTR) continue;
            perror("poll"); result = 1; break;
        }
        // Process existing clients before accepting (push_back can reallocate).
        for (size_t i = clients.size(); i-- > 1;) {
            int fd = clients[i].fd;
            short events = clients[i].revents;
            if (!events) continue;
            bool done = events & (POLLERR | POLLNVAL);
            if (!done && (clients[i].events & POLLIN) && (events & (POLLIN | POLLHUP))) {
                char data[4096];
                ssize_t size = read(fd, data, sizeof(data));
                if (size <= 0) {
                    done = size == 0 || (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK);
                } else {
                    auto &buffer = connections[fd].input;
                    buffer.append(data, static_cast<size_t>(size));
                    auto end = buffer.find("\r\n\r\n");
                    if (buffer.size() > 65536) {
                        const char response[] = "HTTP/1.1 431 Request Header Fields Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                        connections[fd].output.prepare(response);
                        clients[i].events = POLLOUT;
                    } else if (end != std::string::npos) {
                        HttpRequest request;
                        request.setFD(fd);
                        request.outgoing = &connections[fd].output;
                        request.tryDecode(buffer.substr(0, end + 4));
                        auto handler = routes.handler(request.getUrl());
                        if (request.getMethod() != "GET") {
                            const char response[] = "HTTP/1.1 405 Method Not Allowed\r\nAllow: GET\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                            connections[fd].output.prepare(response);
                        } else if (handler) {
                            handler(request);
                        } else {
                            const char response[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
                            connections[fd].output.prepare(response);
                        }
                        clients[i].events = POLLOUT;
                        buffer.clear();
                    }
                }
            }
            if (!done && (events & POLLOUT)) done = connections[fd].output.flush(fd);
            if (!done && (events & POLLHUP) && (clients[i].events & POLLOUT)) done = true;
            if (done) {
                close(fd);
                connections.erase(fd);
                clients.erase(clients.begin() + i);
            }
        }
        if (clients[0].revents & POLLIN) {
            // Bound each accept batch so active connections also get a turn.
            for (int accepted = 0; accepted < 64; ++accepted) {
                int fd = accept(listener, nullptr, nullptr);
                if (fd < 0) {
                    if (errno == EINTR) continue;
                    if (errno != EAGAIN && errno != EWOULDBLOCK) perror("accept");
                    break;
                }
                if (!nonblocking(fd)) { close(fd); continue; }
                setKeepALive(fd, 30, 60, 2);
                clients.push_back({fd, POLLIN, 0});
            }
        }
    }
    for (const auto &client : clients) close(client.fd);
    return result;
}
#endif
