#include "./http/http.h"
#include "./server/server.h"


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
