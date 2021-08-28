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

![](https://s3.bmp.ovh/imgs/2021/08/ada23dfb16839f18.png)

## 详细情况

### 可扩展性，可维护性：

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


### 解析http请求 
使用fsm进行解析，具体参考了[这篇文章](https://www.dazhuanlan.com/chasingsouls/topics/1354274)的思路，在httprequest类中的trydecode方法中实现


### 一个连接下载100M
完成此需求也就意味着完成了1-4的基础需求。
测试用的下载文件就是100m的文件

30分钟压测结果下载带宽约为350mbps。

### 分chunk下载
简单的实现了chunked，逻辑为判断请求头是否含有tranfer-encoding:chunked
若有则传输文件使用以下函数
> 测试方式：在postman或者curl中设置chunked请求头
具体实现见file_service/file_service.h

range 和 chunk在file_service 下均有单元测试

目前的实现只支持tranfer-encoding:chunked和固定chunk大小

### range下载

目前只支持单一范围的range，也就是Range: bytes=0-1023这种形式
实现代码如下
> 测试方式：在postman或者curl中设置range请求头
### 长链接
测试：请求头中加入connection:keep-alive 字段
实现如下：



```c++
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

    int alive = 1;
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
```

### 302 重定向
运行之后输入不存在的url路由，会返回302响应，并在host中指定返回的站点

使用access 函数实现