cmake_minimum_required(VERSION 3.10)
project(httpd)

set(CMAKE_CXX_STANDARD 14)

# add_executable(httpdtest http-test.cpp http/http.h http/http.cpp  socket/set_socket.h)
add_executable(epollserver
 epoll-server.cpp 
 http/http.h 
 http/http.cpp 
 socket/set_socket.h
 file_service/file_service.h
 logger.h
 )
