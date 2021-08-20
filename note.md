server.GET("uri", [](context &) {

});
void func(context&)
{

}

server.GET(const char*, std::function<void(context&)>);
server.STATICFILE(const char* uri, const char* local_path);
request
response
server
context {request, response};
route

s.GET()
s.HEAD()
s.POST()
s.STATIC()
s.STATICFILE()