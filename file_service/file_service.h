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
                             {NULL, "text/plain"}};


static inline size_t get_file_size(char *filename)
{
    struct stat st;
    if (stat(filename, &st) < 0) {
        printf("st init err\n");
    };
    return st.st_size;
}

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
static void write_static(int fd,
                         char *filename,
                         int begin_pos,
                         int end_pos,
                         string &header)
{
    int src_fd = open(filename, O_RDONLY, 0);
    if (src_fd < 2)
        printf("open %s error\n", filename);
    // TODO: sendfile（）
    char buffer[NORMAL_SIZE];
    memset(buffer, 0, NORMAL_SIZE);

    // 传输header
    write(fd, header.c_str(), header.size());

    if (begin_pos != 0) {
        lseek(src_fd, begin_pos * NORMAL_SIZE, SEEK_SET);
    }

    int rcnt = 0;
    int cnt = 0;
    if (end_pos <= 0) {
        do {
            rcnt = read(src_fd, buffer, NORMAL_SIZE);
            int res = write(fd, buffer, rcnt);
        } while (rcnt == NORMAL_SIZE);
    } else {
        int size = end_pos - begin_pos;
        do {
            rcnt = read(src_fd, buffer, NORMAL_SIZE);
            write(fd, buffer, rcnt);
            size -= NORMAL_SIZE;
        } while (size > 0);
    }
    close(src_fd);
    close(fd);
}

const char *get_file_type(char * filename)
{
    string fa(filename);
    string suffixStr = fa.substr(fa.find_last_of('.'));  //获取文件后缀
    if (suffixStr.size() == 0)
        return "text/plain";

    int i;
    for (i = 0; mime[i].type; ++i) {
        if (!strcmp(suffixStr.c_str(), mime[i].type))
            return mime[i].value;
    }
    return mime[i].value;
}



#endif