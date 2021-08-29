#ifndef __SERVER__
#define __SERVER__

#include <iostream>
#include "mainloop.h"
#include "route.h"

using namespace httprouter;

class HttpServer
{
public:
    // itor
    int GET(char *url_pattern, handler_t callback)
    {
        no.addRoute(url_pattern, callback);
    };
    int HEAD(char *url_pattern, handler_t callback);
    int STATIC(char *url_pattern, handler_t callback)
    {
        no.addRoute(url_pattern, callback);
    };
    void bind(char *ip)
    {
        strcpy(this->ip, ip);
    }
    void listen(int port)
    {
        this->port = port;
    };

    void Run()
    {
        mainloop(ip, port, no);
    }

private:
    node no;
    char *ip;
    int port = 8888;
};
#endif
