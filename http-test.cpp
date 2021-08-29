#include "http.h"
#include <assert.h>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    std::string str =
        "POST /audiolibrary/music?ar=1595301089068&n=1p1 HTTP/1.1\r\n"
        "content-length:28\r\n"
        "Host: www.google.cn\r\n"
        "Connection: Keep-Alive\r\n"
        "Range: bytes=0-1023\r\n"
        "hl=zh-CN&source=hp&q=domety\r\n\r\n";
    HttpRequest req;
    req.tryDecode(str);
    HttpResponse res(req, 2, (char *) "dwefer.mp4");
    string t("fbnerjfer");
    res.setBody(t);
    cout << res.getHeaderStr();
    return 0;
}
