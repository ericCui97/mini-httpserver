#ifndef __SERVER__
#define __SERVER__

#include <iostream>
#include "../mainloop.h"
#include "route.h"

using namespace httprouter;

class HttpServer
{
public:
    //! ctor
    HttpServer(){

    };
    // !dtor
    ~HttpServer(void);
    int GET(char *url_pattern, handler_t callback)
    {
        no.addRoute(url_pattern, callback);
    };
    int HEAD(char *url_pattern, handler_t callback);
    int STATIC(char *url_pattern, handler_t callback)
    {
        no.addRoute(url_pattern, callback);
    };

    void Run(const char *ip = "127.0.0.1", const int port = 80)
    {
        mainloop(ip, port, no);
    }

private:
    node no;
    int port;
};
#endif
