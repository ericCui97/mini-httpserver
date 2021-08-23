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
#define NORMAL_SIZE 4096
using namespace std;

static inline size_t get_file_size(char *filename)
{
    struct stat st;
    if (stat(filename, &st) < 0)
    {
        printf("st init err\n");
    };
    return st.st_size;
}
/**
/** @brief
/  *
/  * @param fd  发送的文件的fd
/  * @param filename 文件路径
/  * @param  header response的header
/* @param current_pos int 文件偏移量
/  * @return void
/  */

inline static void write_static_chunked(int fd, char *filename, string header, int current_pos, int CHUNK_SIZE)
{
    int src_fd = open(filename, O_RDONLY, 0);
    if (src_fd < 2)
        printf("open %s error\n", filename);
    // TODO: sendfile（）
    char buffer[CHUNK_SIZE];
    memset(buffer, 0, CHUNK_SIZE);

    // size_t uint
    write(fd, header.c_str(), header.size());

    if (current_pos != 0)
    {
        lseek(src_fd, current_pos*CHUNK_SIZE, SEEK_SET);
    }

    int rcnt = 0;
    int cnt = 0;
    do
    {
        rcnt = read(src_fd, buffer, CHUNK_SIZE);
        write(fd, buffer, rcnt);
    } while (rcnt == CHUNK_SIZE);

    close(src_fd);
    close(fd);
}

inline static void write_static_uchunked(int fd, char *filename, string header, int current_pos)
{
    write_static_chunked(fd, filename, header, current_pos, get_file_size(filename));
}

#endif