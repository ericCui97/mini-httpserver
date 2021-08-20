#ifndef __file_service__
#define __file_service__
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "logger.h"

using namespace std;

// size_t get_file_size(char *filename);

// static inline ssize_t writen(int fd, char *usrbuff, size_t file_size)
// {
//     ssize_t nwritten;
//     char *bufp = usrbuff;
//     for (size_t nleft = file_size; nleft > 0; nleft -= nwritten) {
//         if ((nwritten = write(fd, bufp, nleft)) <= 0) {
//             if (errno == EINTR)
//                 nwritten = 0;
//             else {
//                 log_err("errno == %d\n", errno);
//                 return -1;
//             }
//         }
//         bufp += nwritten;
//     }

//     return file_size;
// }

// static inline size_t batch_write(int fd,
//                                  char *buffer,
//                                  size_t batch_size,
//                                  char *filename)
// {
//     size_t file_size = get_file_size(filename);
//     size_t written_size;
//     size_t nwritten;
//     char *bufp = buffer;
//     for (size_t nleft = file_size; nleft > 0; nleft -= nwritten) {
//         if ((nwritten = write(fd, bufp, nleft)) <= 0) {
//             if (errno == EINTR)
//                 nwritten = 0;
//             else {
//                 log_err("errno == %d\n", errno);
//                 return -1;
//             }
//         }
//         bufp += nwritten;
//     }

//     return file_size;
// }

// static inline size_t get_file_size(char *filename)
// {
//     struct stat st;
//     if (stat(filename, &st) < 0) {
//         printf("st init err\n");
//     };
//     return st.st_size;
// }
// /**
//  * @brief
//  *
//  * @param fd  发送的文件的fd
//  * @param filename 文件路径
//  * @param  header response的header
//  * @return void
//  */



static void write_static(int fd, char *filename, string header)
{
    // size_t file_size = get_file_size(filename);
    int src_fd = open(filename, O_RDONLY, 0);
    if (src_fd < 2)
        printf("open %s error\n", filename);
    // TODO: sendfile（）
    // char *src_addr =
    //     (char *) mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    // close(src_fd);
    // writen(fd, src_addr, file_size);

    const int buffer_size = 4096;
    char buffer[buffer_size];
    memset(buffer, 0, buffer_size);

    // size_t uint
    write(fd, header.c_str(), header.size());

    int rcnt = 0;
    int cnt = 0;
    do {
        rcnt = read(src_fd, buffer, buffer_size);
        cnt++;
        printf("read size: %d, cnt: %d\n", rcnt, cnt);
        write(fd, buffer, rcnt);
    } while (rcnt == buffer_size);


    close(src_fd);
    close(fd);
}



#endif