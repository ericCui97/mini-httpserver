

#include <iostream>
#include "route.h"

void hello1()
{
    std::cout << "hello1" << std::endl;
}
void hello2()
{
    std::cout << "hello2" << std::endl;
}
void hello3()
{
    std::cout << "hello3" << std::endl;
}
void hello4()
{
    std::cout << "hello4" << std::endl;
}
void hello5()
{
    std::cout << "hello5" << std::endl;
}

int main()
{
    httprouter::node no;

    no.addRoute("/", hello1);
    no.addRoute("/serach/", hello2);
    no.addRoute("/support/", hello3);
    no.addRoute("/blog/", hello4);

    no.handler("/")();
    no.handler("/serach/")();
    no.handler("/support/")();
    no.handler("/blog/")();
}
