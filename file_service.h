#ifndef __file_service__
#define __file_service__
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "logger.h"
#define NORMAL_SIZE 4096
using namespace std;
using std::string;

typedef struct {
    const char *type;
    const char *value;
} mime_type_t;

static mime_type_t mime[] = {{".html", "text/html"},
                             {".xml", "text/xml"},
                             {".xhtml", "application/xhtml+xml"},
                             {".txt", "text/plain"},
                             {".pdf", "application/pdf"},
                             {".png", "image/png"},
                             {".gif", "image/gif"},
                             {".jpg", "image/jpeg"},
                             {".css", "text/css"},
                             {".mp4", "application/octet-stream"},
                             {NULL, "text/plain"}};

static void write_chunked(int fd, char *filename, int C_SIZE, string &header);

static inline size_t get_file_size(const char *filename)
{
    struct stat st;
    if (stat(filename, &st) < 0) {
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

#define FIlEEND -1

static void write_static(int fd,
                         char *filename,
                         int begin_pos,
                         int end_pos,
                         string &header)
{
    int src_fd = open(filename, O_RDONLY, 0);
    int file_size = get_file_size(filename);
    if (src_fd < 2)
        printf("open %s error\n", filename);
    // TODO: sendfile（）
    char buffer[NORMAL_SIZE];
    memset(buffer, 0, NORMAL_SIZE);

    // 传输header
    int wcnt = write(fd, header.c_str(), header.size());
    cout << "wcnt:" << wcnt << endl;


    if (begin_pos != 0) {
        lseek(src_fd, begin_pos * NORMAL_SIZE, SEEK_SET);
    }
    int rcnt = 0;
    int cnt = 0;
    int total = 0;
    if (end_pos <= 0) {
        do {
            rcnt = read(src_fd, buffer, NORMAL_SIZE);

            file_size -= NORMAL_SIZE;
            int res = write(fd, buffer, rcnt);

        } while (rcnt == NORMAL_SIZE && file_size > 0);
    } else {
        int size = end_pos - begin_pos;
        do {
            rcnt = read(src_fd, buffer, NORMAL_SIZE);
            write(fd, buffer, rcnt);
            size -= NORMAL_SIZE;

        } while (size > 0);
    }
    close(src_fd);
}


// static const char *get_file_type(const char *filename)
// {
//     string fa(filename);
//     string suffixStr = fa.substr(fa.find_last_of('.'));
//     //获取文件后缀
//     printf("suffixStr:%s", suffixStr);
//     if (suffixStr.size() == 0)
//         return "application/octet-stream";

//     int i;
//     for (i = 0; mime[i].type; ++i) {
//         if (!strcmp(suffixStr.c_str(), mime[i].type))
//             return mime[i].value;
//     }
//     return mime[i].value;
// }
static void write_chunked(int fd, char *filename, int C_SIZE, string &header)
{
    int src_fd = open(filename, O_RDONLY, 0);
    int file_size = get_file_size(filename);
    std::cout << file_size << endl;
    if (src_fd < 2)
        printf("open %s error\n", filename);
    // TODO: sendfile（）
    char chunk[C_SIZE + 2];
    memset(chunk, 0, C_SIZE + 2);
    // 传输header
    write(fd, header.c_str(), header.size());
    int rcnt = 0;
    const char *eof = "0\r\n\r\n";

    char s[6];
    // itoa(C_SIZE, s, 10);
    sprintf(s, "%d", C_SIZE);
    s[4] = '\r';
    s[5] = '\n';
    // const char *s = std::to_string(C_SIZE).c_str() + '\r' + '\n';

    std::cout << "length of s:" << strlen(s) << std::endl;
    while (1) {
        if (file_size <= C_SIZE) {
            rcnt = read(src_fd, chunk, file_size);
            const char *ss = std::to_string(file_size).c_str() + '\r' + '\n';
            write(fd, s, strlen(ss));
            write(fd, chunk, rcnt);
            write(fd, eof, strlen(eof));
            break;
        } else {
            rcnt = read(src_fd, chunk, C_SIZE);
            write(fd, s, strlen(s));
            chunk[C_SIZE] = '\r';
            chunk[C_SIZE + 1] = '\n';
            // cout << "chunksize:" << strlen(chunk) << endl;
            write(fd, chunk, strlen(chunk));
        }
        file_size -= C_SIZE;
    }
    close(src_fd);
}


// 9\r\n
// Mozilla\r\n
// 9\r\n
// Developer\r\n
// 7\r\n
// Network\r\n
// 0\r\n
// \r\n
#endif