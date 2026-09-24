#include "../outgoing.h"
#include <cassert>
#include <csignal>
#include <sys/socket.h>

int main() {
    signal(SIGPIPE, SIG_IGN);
    char path[] = "/tmp/mini-http-outgoing-XXXXXX";
    int source = mkstemp(path);
    assert(source >= 0);
    std::string data;
    for (int i = 0; i < 700000; ++i) data += static_cast<char>(i % 251);
    assert(write(source, data.data(), data.size()) == static_cast<ssize_t>(data.size()));
    close(source);
    for (bool chunked : {false, true}) {
        int sockets[2];
        assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0);
        for (int fd : sockets) assert(fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
        int size = 4096;
        assert(setsockopt(sockets[0], SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) == 0);
        Outgoing output;
        output.prepare("header\r\n\r\n", path, 7, 699990, chunked);
        assert(!output.flush(sockets[0]));
        // The peer has not drained anything: EAGAIN must preserve the cursor.
        assert(!output.flush(sockets[0]));
        std::string received;
        bool finished = false;
        for (int attempt = 0; attempt < 10000; ++attempt) {
            if (!finished) finished = output.flush(sockets[0]);
            char buffer[3000];
            ssize_t count;
            while ((count = read(sockets[1], buffer, sizeof(buffer))) > 0)
                received.append(buffer, count);
            if (finished) break;
        }
        assert(finished);
        std::string expected = "header\r\n\r\n";
        auto body = data.substr(7, 699990 - 7 + 1);
        if (!chunked) expected += body;
        else {
            for (size_t offset = 0; offset < body.size(); offset += 65536) {
                auto block = body.substr(offset, 65536);
                char length[32];
                snprintf(length, sizeof(length), "%zx\r\n", block.size());
                expected += length + block + "\r\n";
            }
            expected += "0\r\n\r\n";
        }
        assert(received == expected);
        close(sockets[0]); close(sockets[1]);
    }
    unlink(path);
    puts("PASS: partial writes, EAGAIN resume, patterned byte ranges, chunk framing");
}
