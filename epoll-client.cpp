#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

#define MAXSIZE 1024
#define IP_ADDR "127.0.0.1"
#define IP_PORT 8888

int i_sockfd = -1;

void SigCatch(int sigNum)	//信号捕捉函数(捕获Ctrl+C)
{
	if(i_sockfd != -1)
	{
		close(i_sockfd);
	}
	printf("Bye~! Will Exit...\n");
	exit(0);
}

int main()
{
	struct sockaddr_in st_clnsock;
	char msg[1024];
	int nrecvSize = 0;

	signal(SIGINT, SigCatch);	//注册信号捕获函数

	if((i_sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0)	//建立套接字
	{
		printf("socket Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&st_clnsock, 0, sizeof(st_clnsock));
	st_clnsock.sin_family = AF_INET;  //IPv4协议
	//IP地址转换(直接可以从物理字节序的点分十进制 转换成网络字节序)
	if(inet_pton(AF_INET, IP_ADDR, &st_clnsock.sin_addr) <= 0)
	{
		printf("inet_pton Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	st_clnsock.sin_port = htons(IP_PORT);	//端口转换(物理字节序到网络字节序)

	if(connect(i_sockfd, (struct sockaddr*)&st_clnsock, sizeof(st_clnsock)) < 0)	//主动向设置的IP和端口号的服务端发出连接
	{
		printf("connect Error: %s (errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	printf("======connect to server, sent data======\n");

	while(1)	//循环输入，向服务端发送数据并接受服务端返回的数据
	{
		fgets(msg, MAXSIZE, stdin);
		printf("will send: %s", msg);
		if(write(i_sockfd, msg, MAXSIZE) < 0)	//发送数据
		{
			printf("write Error: %s (errno: %d)\n", strerror(errno), errno);
			exit(0);
		}

		memset(msg, 0, sizeof(msg));
		if((nrecvSize = read(i_sockfd, msg, MAXSIZE)) < 0)	//接受数据
		{
			printf("read Error: %s (errno: %d)\n", strerror(errno), errno);
		}
		else if(nrecvSize == 0)
		{
			printf("Service Close!\n");
		}
		else
		{
			printf("Server return: %s\n", msg);
		}

	}
	return 0;
}
