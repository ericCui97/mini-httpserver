#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
using std::string;

#define MAXLINE 2048
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5555
#define INFTIM 1000
string http_header(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type:text/html\r\n"
    "\r\n\r\n");
void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);

    if (opts < 0) {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }

    opts = opts | O_NONBLOCK;

    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock, SETFL, opts)");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    printf("epoll socket begins.\n");
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;

    struct epoll_event ev, events[20];

    epfd = epoll_create(256);

    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    setnonblocking(listenfd);

    ev.data.fd = listenfd;
    ev.events = EPOLLIN | EPOLLET;

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    listen(listenfd, LISTENQ);

    maxi = 0;

    for (;;) {
        nfds = epoll_wait(epfd, events, 20, 500);

        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listenfd) {
                printf("accept connection, fd is %d\n", listenfd);
                connfd =
                    accept(listenfd, (struct sockaddr *) &clientaddr, &clilen);
                if (connfd < 0) {
                    perror("connfd < 0");
                    exit(1);
                }

                setnonblocking(connfd);

                char *str = inet_ntoa(clientaddr.sin_addr);
                printf("connect from %s\n", str);

                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                if ((sockfd = events[i].data.fd) < 0)
                    continue;
                if ((n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else {
                        printf("readline error");
                    }
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }

                printf(" %s", line);

                ev.data.fd = sockfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            } else if (events[i].events & EPOLLOUT) {
                sockfd = events[i].data.fd;

                string content("<h1>dsedf</h1>");

                http_header.append(content);
                write(sockfd, http_header.c_str(), http_header.size());
                printf("written data: %s\n", line);
				printf("client[%d] disconnected!\n",sockfd);
                ev.data.fd = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
}