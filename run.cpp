#include "http.h"
#include "server.h"
int main()
{
    HttpServer server;
    server.bind("127.0.0.1");
    server.listen(9999);

    server.GET("/hello", [](HttpRequest &req)
               {
                   HttpResponse res(req, HTML_TYPE, "hello.html");
                   SEND(res);
               });
    server.GET("/100M", [](HttpRequest &req)
               {
                   HttpResponse res(req, SOCK_STREAM, "100M");
                   SEND(res);
               });
    server.Run();
}
