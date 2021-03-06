
#ifndef __main_loop__
#define __main_loop__

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>  //epoll头文件
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <map>

#include <iostream>
#include "route.h"
#include "set_socket.h"
#include "file_service.h"
#include "http.h"
#include "logger.h"
#define MAXSIZE 1024
 #define IP_ADDR "127.0.0.1"
 #define IP_PORT 9999


using namespace std;
using namespace httprouter;


static volatile bool keep_running = true;
string url;

void sig_handler(int sig)
{
    if (sig == SIGINT) {
        keep_running = false;
    }
}
static int sock_set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        log_err("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    int s = fcntl(fd, F_SETFL, flags);
    if (s == -1) {
        log_err("fcntl");
        return -1;
    }
    return 0;
}

int mainloop(const char *server_ip, const int port, node &no)
{
    map<int, HttpRequest> rmap;
    signal(SIGINT, sig_handler);  //注册信号，实现 ctrl c 退出，释放端口
    int i_listenfd, i_connfd;
    struct sockaddr_in st_sersock;
    char msg[MAXSIZE];
    int nrecvSize = 0;
    struct epoll_event ev, events[MAXSIZE];
    int epfd, nCounts;  // epfd:epoll实例句柄, nCounts:epoll_wait返回值
    int optval = 1;

    if ((i_listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //建立socket套接字
    {
        printf("socket Error: %s (errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    //解决 Address already in use 问题
    if (setsockopt(i_listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval,
                   sizeof(int)) < 0)
        return -1;

    memset(&st_sersock, 0, sizeof(st_sersock));
    st_sersock.sin_family = AF_INET;  // IPv4协议
    st_sersock.sin_addr.s_addr = htonl(
        INADDR_ANY);  // INADDR_ANY转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP，因为有些机子不止一块网卡，多网卡的情况下，这个就表示所有网卡ip地址的意思。
    st_sersock.sin_port = htons(IP_PORT);
    std::cout<<st_sersock.sin_port<<endl;

    if (bind(i_listenfd, (struct sockaddr *) &st_sersock, sizeof(st_sersock)) <
        0)  //将套接字绑定IP和端口用于监听
    {
        printf("bind Error: %s (errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    if (listen(i_listenfd, 20) < 0)  //设定可同时排队的客户端最大连接个数
    {
        printf("ferjgfier\n");
        printf("listen Error: %s (errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    if ((epfd = epoll_create(MAXSIZE)) < 0)  //创建epoll实例
    {
        printf("epoll_create Error: %s (errno: %d)\n", strerror(errno), errno);
        exit(-1);
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = i_listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, i_listenfd, &ev) < 0) {
        printf("epoll_ctl Error: %s (errno: %d)\n", strerror(errno), errno);
        exit(-1);
    }
    printf("======waiting for client's request======\n");
    //准备接受客户端连接
    while (keep_running) {
        if ((nCounts = epoll_wait(epfd, events, MAXSIZE, -1)) < 0) {
            printf("epoll_ctl Error: %s (errno: %d)\n", strerror(errno), errno);
            exit(-1);
        } else if (nCounts == 0) {
            printf("time out, No data!\n");
        } else {
            for (int i = 0; i < nCounts; i++) {
                int tmp_epoll_recv_fd = events[i].data.fd;
                if (tmp_epoll_recv_fd == i_listenfd)  //有客户端连接请求
                {
                    if ((i_connfd = accept(i_listenfd, (struct sockaddr *) NULL,
                                           NULL)) < 0)  //阻塞等待客户端连接
                    {
                        printf("accept Error: %s (errno: %d)\n",
                               strerror(errno), errno);
                        continue;
                    } else {
                        printf("Client[%d], welcome!\n", i_connfd);
                    }

                    ev.events = EPOLLIN | EPOLLET | EPOLLERR;
                    ev.data.fd = i_connfd;
                    setKeepALive(i_connfd, 30, 60, 2);
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, i_connfd, &ev) < 0) {
                        printf("epoll_ctl Error: %s (errno: %d)\n",
                               strerror(errno), errno);
                        exit(-1);
                    }
                } else if (events[i].events == EPOLLIN) {
                    bool is_keep_alive = false;
                    memset(msg, 0, sizeof(msg));
                    printf("client %d entry!\n", tmp_epoll_recv_fd);
                    if ((nrecvSize = read(tmp_epoll_recv_fd, msg, MAXSIZE)) <=
                        0) {
                        printf("client has disconnected!\n");
                        ev.events = EPOLLIN | EPOLLOUT;
                        ev.data.fd = tmp_epoll_recv_fd;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, tmp_epoll_recv_fd, &ev);
                        shutdown(tmp_epoll_recv_fd, SHUT_WR | SHUT_RD);  //

                        continue;
                    }

                    ev.events = EPOLLOUT | EPOLLET;
                    ev.data.fd = tmp_epoll_recv_fd;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, tmp_epoll_recv_fd, &ev);

                    HttpRequest req;
                    req._fd=tmp_epoll_recv_fd;
                    req.tryDecode(msg);
                    rmap.emplace(tmp_epoll_recv_fd, req);
                } else if (events[i].events == EPOLLOUT) {
                    HttpRequest req = rmap[tmp_epoll_recv_fd];
                    no.handler(req.getUrl())(req);

                    // cout << "tmp_epoll_recv_fd" << tmp_epoll_recv_fd << endl;
                    // string header(
                    //     "HTTP/1.1 200 OK\r\n"
                    //     "Content-Type:application/octet-stream\r\n"
                    //     "Content-Disposition:attachment;"
                    //     "filename=test.mp4\r\n"
                    //     "\r\n");
                    // write_static(tmp_epoll_recv_fd, "./test.mp4", 0, FIlEEND,
                    //              header);
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = tmp_epoll_recv_fd;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, tmp_epoll_recv_fd, &ev);
                    // // if (!is_keep_alive) {
                    close(tmp_epoll_recv_fd);
                    // // }
                } else if (events[i].events & EPOLLERR) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    printf("client close , because of err \n");
                    continue;
                }
            }
        }
    }  // while
    close(i_listenfd);
    close(epfd);
    return 0;
}

#endif