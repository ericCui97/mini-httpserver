cmake_minimum_required(VERSION 3.10)
project(httpd)

set(CMAKE_CXX_STANDARD 14)

# add_executable(httpdtest http/http-test.cpp http/http.h http/http.cpp  socket/set_socket.h file_service/file_service.h)
add_executable(epollserver
mainloop.h
run.cpp
server/route.h
server/server.h
server/route.cpp
 http/http.h 
 http/http.cpp 
 socket/set_socket.h
 file_service/file_service.h
 logger.h
 )
