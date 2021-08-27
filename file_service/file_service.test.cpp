#include "./file_service.h"
#include <assert.h>
#include <cstdio>

int main()
{
    string header(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type:application/octet-stream\r\n"
        "Content-Disposition:attachment; "
        "filename=test.mp4\r\n"
        "Connection:Keep-Alive"
        "\r\n\r\n");
    int dest_fd = open("./output.txt", O_RDWR);
    write_chunked(dest_fd, "test.txt", 1280, header);
    close(dest_fd);
    return 0;
}
