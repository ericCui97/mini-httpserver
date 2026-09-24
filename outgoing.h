#ifndef MINI_HTTP_OUTGOING_H
#define MINI_HTTP_OUTGOING_H
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

// One bounded buffer and file cursor per connection; never buffer a whole file.
class Outgoing {
public:
    Outgoing() = default;
    Outgoing(const Outgoing &) = delete;
    Outgoing &operator=(const Outgoing &) = delete;
    ~Outgoing() { if (file >= 0) close(file); }

    void prepare(std::string header, const char *filename = nullptr,
                 int first = 0, int last = -1, bool chunks = false) {
        pending = std::move(header);
        chunked = chunks;
        if (!filename) return;
        file = open(filename, O_RDONLY);
        struct stat st{};
        if (file < 0 || fstat(file, &st) < 0 || lseek(file, first, SEEK_SET) < 0) {
            if (file >= 0) close(file);
            file = -1;
            pending = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
            chunked = false;
            return;
        }
        remaining = last >= 0 ? last - first + 1 : st.st_size - first;
    }

    // true means finished or failed; false means resume on POLLOUT.
    bool flush(int socket) {
        size_t budget = 256 * 1024;
        while (budget) {
            if (offset < pending.size()) {
                ssize_t count = write(socket, pending.data() + offset,
                                      std::min(budget, pending.size() - offset));
                if (count < 0 && errno == EINTR) continue;
                if (count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return false;
                if (count <= 0) return true;
                offset += count;
                budget -= count;
                continue;
            }
            pending.clear();
            offset = 0;
            if (remaining <= 0) {
                if (chunked) { pending = "0\r\n\r\n"; chunked = false; continue; }
                return true;
            }
            char buffer[64 * 1024];
            ssize_t count = read(file, buffer, std::min<off_t>(sizeof(buffer), remaining));
            if (count < 0 && errno == EINTR) continue;
            if (count <= 0) return true;
            remaining -= count;
            if (chunked) {
                char length[32];
                snprintf(length, sizeof(length), "%zx\r\n", static_cast<size_t>(count));
                pending = length;
            }
            pending.append(buffer, count);
            if (chunked) pending += "\r\n";
        }
        return false;
    }
private:
    int file = -1;
    off_t remaining = 0;
    std::string pending;
    size_t offset = 0;
    bool chunked = false;
};
#endif
