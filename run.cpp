#include "http.h"
#include "server.h"
int main()
{
    HttpServer server;
    server.bind("127.0.0.1");
    server.listen(9999);

    server.GET("/", [](HttpRequest &req) {
        HttpResponse res(req, HTML_TYPE, "hello.html");
        SEND(res);
    });
    server.GET("/hello", [](HttpRequest &req)
               {
                   HttpResponse res(req, HTML_TYPE, "hello.html");
                   SEND(res);
               });
    server.GET("/100M", [](HttpRequest &req)
               {
                   HttpResponse res(req, BINARY_TYPE, "100M");
                   SEND(res);
               });
    return server.Run();
}
