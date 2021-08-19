//////////////////////////////////////////
//服务器代码
//////////////////////////////////////////
//本文件是服务器的代码
#include <netinet/in.h>  // for sockaddr_in
#include <stdio.h>       // for printf
#include <stdlib.h>      // for exit
#include <string.h>      // for bzero
#include <sys/socket.h>  // for socket
#include <sys/types.h>   // for socket
#include <time.h>        //for time_t and time
#define HELLO_WORLD_SERVER_PORT 7754
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
int main(int argc, char **argv)
{
    //设置一个socket地址结构server_addr,代表服务器internet地址, 端口
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));  //把一段内存区的内容全部设置为0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    // time_t now;
    FILE *stream;
    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Create Socket Failed!");
        exit(1);
    }
    //把socket和socket地址结构联系起来
    if (bind(server_socket, (struct sockaddr *) &server_addr,
             sizeof(server_addr))) {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);
        exit(1);
    }
    // server_socket用于监听
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE)) {
        printf("Server Listen Failed!");
        exit(1);
    }
    while (1)  //服务器端要一直运行
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        int new_server_socket =
            accept(server_socket, (struct sockaddr *) &client_addr, &length);
        if (new_server_socket < 0) {
            printf("Server Accept Failed!\n");
            break;
        }
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        strcpy(buffer, "Hello,World! 从服务器来！");
        strcat(buffer, "\n");  // C语言字符串连接
        send(new_server_socket, buffer, BUFFER_SIZE, 0);
        bzero(buffer, BUFFER_SIZE);
        //接收客户端发送来的信息到buffer中
        length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);
        if (length < 0) {
            printf("Server Recieve Data Failed!\n");
            exit(1);
        }
        printf("\n%s", buffer);
    
        if ((stream = fopen("data1", "r")) == NULL) {
            printf("The file 'data1' was not opened! \n");
            exit(1);
        } else
            printf("The file 'filename' was opened! \n");
        bzero(buffer, BUFFER_SIZE);
        int lengsize = 0;
        while ((lengsize = fread(buffer, 1, 1024, stream)) > 0) {
            printf("lengsize = %d\n", lengsize);
            if (send(new_server_socket, buffer, lengsize, 0) < 0) {
                printf("Send File is Failed\n");
                break;
            }
            bzero(buffer, BUFFER_SIZE);
        }
        if (fclose(stream))
            printf("The file 'data' was not closed! \n");
        exit(1);
        //关闭与客户端的连接
        close(new_server_socket);
    }
    //关闭监听用的socket
    close(server_socket);
    return 0;
}