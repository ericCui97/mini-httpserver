#ifndef MINI_HTTP_FILE_SERVICE_H
#define MINI_HTTP_FILE_SERVICE_H
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#define NORMAL_SIZE 4096
#define FIlEEND -1

static inline size_t get_file_size(const char *filename)
{
    struct stat st{};
    return stat(filename, &st) == 0 ? static_cast<size_t>(st.st_size) : 0;
}

static bool write_all(int fd, const char *data, size_t size)
{
    while (size > 0) {
        ssize_t count = write(fd, data, size);
        if (count < 0 && errno == EINTR) continue;
        if (count <= 0) return false;
        data += count;
        size -= static_cast<size_t>(count);
    }
    return true;
}

static void write_static(int fd, const char *filename, int begin_pos,
                         int end_pos, std::string &header)
{
    int source = open(filename, O_RDONLY);
    if (source < 0) {
        const char response[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        write_all(fd, response, sizeof(response) - 1);
        return;
    }
    if (lseek(source, begin_pos, SEEK_SET) < 0 ||
        !write_all(fd, header.data(), header.size())) {
        close(source);
        return;
    }
    size_t remaining = end_pos >= 0 ? static_cast<size_t>(end_pos - begin_pos + 1)
                                   : get_file_size(filename) - begin_pos;
    char buffer[NORMAL_SIZE];
    while (remaining > 0) {
        ssize_t count = read(source, buffer, std::min(remaining, sizeof(buffer)));
        if (count < 0 && errno == EINTR) continue;
        if (count <= 0 || !write_all(fd, buffer, static_cast<size_t>(count))) break;
        remaining -= static_cast<size_t>(count);
    }
    close(source);
}

static void write_chunked(int fd, const char *filename, int chunk_size,
                          std::string &header)
{
    int source = open(filename, O_RDONLY);
    if (source < 0) return;
    std::vector<char> buffer(chunk_size);
    if (write_all(fd, header.data(), header.size())) {
        while (true) {
            ssize_t count = read(source, buffer.data(), buffer.size());
            if (count < 0 && errno == EINTR) continue;
            if (count < 0) break;
            if (count == 0) { write_all(fd, "0\r\n\r\n", 5); break; }
            char length[32];
            int size = snprintf(length, sizeof(length), "%zx\r\n", static_cast<size_t>(count));
            if (!write_all(fd, length, size) || !write_all(fd, buffer.data(), count) ||
                !write_all(fd, "\r\n", 2)) break;
        }
    }
    close(source);
}
#endif
