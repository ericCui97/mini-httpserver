server.GET("uri", [](context &) {

});
void func(context&)
{

}

server.GET(const char*, std::function<void(context&)>);
server.STATICFILE(const char* uri, const char* local_path);
request
response
server
context {request, response};
route

s.GET()
s.HEAD()
s.POST()
s.STATIC()
s.STATICFILE()

EPOLLOUT事件：
EPOLLOUT事件只有在连接时触发一次，表示可写，其他时候想要触发，那你要先准备好下面条件：
1.某次write，写满了发送缓冲区，返回错误码为EAGAIN。
2.对端读取了一些数据，又重新可写了，此时会触发EPOLLOUT。
简单地说：EPOLLOUT事件只有在不可写到可写的转变时刻，才会触发一次，所以叫边缘触发，这叫法没错的！



// siege