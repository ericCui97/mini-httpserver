cmake_minimum_required(VERSION 3.10)
project(httpd)

set(CMAKE_CXX_STANDARD 14)

add_executable(httpd http-test.cpp http.h http.cpp)
add_executable(epollserver epoll-server.cpp http.h http.cpp)
