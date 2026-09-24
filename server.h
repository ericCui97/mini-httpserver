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
    int GET(const char *url_pattern, handler_t callback)
    {
        no.addRoute(url_pattern, callback);
        return 0;
    };
    int HEAD(const char *url_pattern, handler_t callback);
    int STATIC(const char *url_pattern, handler_t callback)
    {
        no.addRoute(url_pattern, callback);
        return 0;
    };
    void bind(const char *ip)
    {
        this->ip = ip;
    }
    void listen(int port)
    {
        this->port = port;
    };

    int Run()
    {
        return mainloop(ip.c_str(), port, no);
    }

private:
    node no;
    std::string ip = "127.0.0.1";
    int port = 8888;
};
#endif
