#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q) == false){return -1;}

class TcpSocket{
    public:
        TcpSocket() : _sockfd(-1){}
        void SetSockFd(int fd){
            _sockfd = fd;
        }
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
        bool Bind(std::string &ip, uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());

            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        bool Listen(int backlog = 10) {
            //int listen(int sockfd, int backlog);
            //backlog:最大并发连接数--内核中已完成连接队列的最大节点数
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return  true;
        }
        bool Connect(std::string &ip, uint16_t port) {
            //int connect(int fd, struct sockaddr *addr,socklen_t len);
            //addr: 要连接的服务端地址信息
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);

            int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("connect error");
                return false;
            }
            return true;
        }
        bool Accept(TcpSocket &csock, struct sockaddr_in *addr = NULL){
            //int accept(int sockfd, sockaddr *addr, socklen_t *len);
            //addr: 客户端的地址信息
            //len： 输入输出参数，既要指定接收长度，还要接收实际长度
            //返回值：为新客户端新建的socket套接字描述符
            //通过这个返回的描述符可以与指定的客户端进行通信
            struct sockaddr_in _addr;
            socklen_t len = sizeof(struct sockaddr_in);
            int newfd = accept(_sockfd, (struct sockaddr*)&_addr, &len);
            if (newfd < 0) {
                perror("accept error");
                return false;
            }
            if (addr != NULL) {
                memcpy(addr, &_addr, len);
            }
            csock.SetSockFd(newfd);
            //_sockfd--仅用于接收新客户端连接请求
            //newfd----专门用于与客户端进行通信
            return true;
        }
        bool Recv(std::string &buf) {
            char tmp[4096] = {0};
            //ssize_t recv(int sockfd, void *buf, size_t len, int flags)
            //flags:0-默认阻塞接收  MSG_PEEK-获取数据但是不从缓冲区移除
            //返回值：实际接收的数据长度    失败：-1    连接断开：0
            int ret = recv(_sockfd, tmp, 4096, 0);
            if (ret < 0) {
                perror("recv error");
                return false;
            }else if (ret == 0) {
                printf("peer shutdown\n");
                return false;
            }
            buf.assign(tmp, ret);
            return true;
        }
        bool Send(std::string &buf) {
            //ssize_t send(int sockfd, void *buf, size_t len, int flags)
            int ret = send(_sockfd, buf.c_str(), buf.size(), 0);
            if (ret < 0) {
                perror("send error");
                return false;
            }
            return true;
        }
        bool Close() {
            close(_sockfd);
            _sockfd = -1;
        }
    private:
        int _sockfd;
};

