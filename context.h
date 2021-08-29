#ifndef __CONTEXT__
#define __CONTEXT__
#include<memory>
#include<functional>

using handler = std::function<void(Context&)>;
class Context {

public:
    Query(const std::string &item) {
        Reqest.Query(item);
    }

private:
    Request
    Response
};

#endif