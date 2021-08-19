#ifndef __SOCKET_SET__
#define __SOCKET_SET__
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "logger.h"

// class socket_decorator
// {
// public:
//     // static inline int setKeepALive(int _fd, int idle, int intv, int cnt);
//     // static inline int setRange();
//
// };
// /**level=SOL_SOCKET, option_name=SO_KEEPALIVE, option_value=1*/
// // int setsockopt(int socket, int level, int option_name,
// //             const void *option_value, socklen_t option_len);

// TODO:分chunk，不分chunk
// TODO:PIPLINE
// TODO：302 redirect

/**
 * @brief
 *
 * @param _fd  操作的socket的文件描述符
 * @param idle 出发保活机制的时间
 * @param intv 如果没有收到回应，intv秒后重发
 * @param cnt  连续cnt没有收到保活包，则连接失效
 * @return int
 */
inline static int setKeepALive(int _fd, int idle, int intv, int cnt)
{
    // int setsockopt(int socket, int level, int option_name,
    //    const void *option_value, socklen_t option_len);
    /**
     * @brief 使用 setsockopt 实现 keepalive
     *
     *
     */

    int alive = 1;
    /**
     * @brief 设置_fd 的socket连接为keepalive
     *
     */
    if (setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &alive, sizeof alive) != 0) {
        log_err("set keep alive error");
        return -1;
    }

    if (setsockopt(_fd, SOL_TCP, TCP_KEEPINTVL, &intv, sizeof intv) != 0) {
        log_err("set keep alive error");
        return -1;
    }

    if (setsockopt(_fd, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof idle) != 0) {
        log_err("set keep alive error");
        return -1;
    }
    if (setsockopt(_fd, SOL_TCP, TCP_KEEPCNT, &cnt, sizeof cnt) != 0) {
        log_err("set keep alive error");
        return -1;
    }
    return 0;
}

#endif
