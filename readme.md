# mini-http-server



## 1. [需求文档](https://docs.google.com/presentation/d/1nm4f_xyorsatzBByqdXJqUF1fjEQ9YBJUUoU16lpzQQ/edit#slide=id.ge77af77a78_0_82)

## 2.完成情况

- [x] 支持tcp server 
- [x] http get
- [x] io 多路复用
- [x] 单线程 单机下载带宽
- [x] 无资源泄露
- [x] chunk
- [x] 长连接
- [ ] pipline
- [x] 一个请求下载完100m
- [x] range
- [x] 302

## 运行方式

```bash
cd build
cmake ..
make

./epollserver

```



## 测试结果

![截屏2021-08-27 下午7.04.09](/Users/yirong.cui/Library/Application Support/typora-user-images/截屏2021-08-27 下午7.04.09.png)

## 可扩展性，可维护性：

实现了如下图的调用方式，需要添加接口只需要确定uri和回调函数，不需要关心epoll socket的问题，同时实现了动态路由（trie树）

``` c++
int main()
{
    HttpServer server;
    server.GET("/hello", [](HttpRequest &req) {
        HttpResponse res(req, HTML_TYPE, "hello.html");
        res.send();
    });
    server.GET("/100M", [](HttpRequest &req) {
        HttpResponse res(req, SOCK_STREAM, "100M");
       res.send();
    });
    server.Run();
}

```









