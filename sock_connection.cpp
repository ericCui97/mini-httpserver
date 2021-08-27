#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
 
 
volatile int exitNow=0;
 
 
int main()
{
	int epfd;
	int lisSocket;
	char simpleGlobBuffer[1000];
	struct epoll_event epEvent,recEvent[10];
 
 
	if( (epfd=epoll_create(10))==-1 )
	{
		return 1;
	}
 
 
	if( (lisSocket=socket(AF_INET,SOCK_STREAM,0))==-1 )
	{
		return 2;
	}
 
 
	int flag;
 
 
	if(fcntl(lisSocket,F_GETFL,&flag)==-1)
	{
		return 6;
	}
 
 
	flag|=O_NONBLOCK;
 
 
	if(fcntl(lisSocket,F_SETFL,flag)==-1)
	{
		return 7;
	}
 
 
	sockaddr_in lisAddr;
 
 
	lisAddr.sin_family=AF_INET;
	lisAddr.sin_port=htons(12000);
	lisAddr.sin_addr.s_addr=htonl(INADDR_ANY);
 
 
	int reUseOn=1;
 
 
	if(setsockopt(lisSocket,SOL_SOCKET,SO_REUSEADDR,&reUseOn,sizeof(int))==-1)
	{
		return 3;
	}
 
 
	if(bind(lisSocket,(struct sockaddr*)&lisAddr,sizeof(lisAddr))==-1)
	{
		return 4;
	}
 
 
	if(listen(lisSocket,SOMAXCONN)==-1)
	{
		return 5;
	}
 
 
	epEvent.data.fd=lisSocket;
	epEvent.events=EPOLLIN|EPOLLET|EPOLLERR;
 
 
	if(epoll_ctl(epfd,EPOLL_CTL_ADD,lisSocket,&epEvent)==-1)
	{
		return 20;
	}
 
 
	sockaddr_in clientAddr;
	socklen_t addrLen=sizeof(clientAddr);
 
 
	while(!exitNow)
	{
		int nRec=epoll_wait(epfd,recEvent,10,-1);
 
 
		if(nRec==-1)
		{
			if(errno==EINTR)
			{
				continue;
			}
 
 
			return 8;
		}
		else
		{
			for(int i=0;i<nRec;++i)
			{
				if(recEvent[i].data.fd==lisSocket)
				{
					if(recEvent[i].events&EPOLLERR)
					{
						return 10;
					}
 
 
					int clifd;
 
 
					while( true )
					{
						clifd=accept(lisSocket,(struct sockaddr*)&clientAddr,&addrLen);
 
 
						if(clifd==-1)
						{
							if(errno==EAGAIN)
							{
								break;
							}
							else if(errno==EINTR)
							{
								continue;
							}
							
							return 10;
						}
 
 
						epEvent.data.fd=clifd;
						epEvent.events=EPOLLIN|EPOLLET|EPOLLERR|EPOLLRDHUP;
						
						if(fcntl(clifd,F_GETFL,&flag)==-1)
						{
							return 13;
						}
 
 
						flag|=O_NONBLOCK;
 
 
						if(fcntl(clifd,F_SETFL,flag)==-1)
						{
							return 14;
						}
 
 
						if(epoll_ctl(epfd,EPOLL_CTL_ADD,clifd,&epEvent)==-1)
						{
							return 9;
						}
					}
				}
				else
				{
					if(recEvent[i].events&EPOLLERR || recEvent[i].events&EPOLLRDHUP)
					{
						epoll_ctl(epfd,EPOLL_CTL_DEL,recEvent[i].data.fd,NULL);
						close(recEvent[i].data.fd);
						printf("client close , because of err \n");
						continue;
					}
					
					int nRecv;
 
 
					while(true)
					{
						nRecv=recv(recEvent[i].data.fd,simpleGlobBuffer,1000,0);
						
						if(nRecv==0)
						{
							epoll_ctl(epfd,EPOLL_CTL_DEL,recEvent[i].data.fd,NULL);
							close(recEvent[i].data.fd);
							printf("client close , normal close \n");
							break;
						}
						else if(nRecv==-1)
						{
							if(errno==EAGAIN)
							{
								break;
							}
							else if(errno==EINTR)
							{
								continue;
							}
							else
							{
								return 12;
							}
						}
 
 
						if(write(STDOUT_FILENO,simpleGlobBuffer,nRecv)!=nRecv)
						{
							return 11;
						}
					}
				}
			}
		}
	}
 
 
	close(lisSocket);
	
	return 0;
}