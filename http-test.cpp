#include "http/http.h"

int main(int argc, char *argv[])
{
    std::string str =
        "POST /audiolibrary/music?ar=1595301089068&n=1p1 HTTP/1.1\r\n"
        "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, "
        "application/vnd.ms-excel, application/vnd.ms-powerpoint, "
        "application/msword, application/x-silverlight, "
        "application/x-shockwave-flash\r\n"
        "Referer: http://www.google.cn\r\n"
        "Accept-Language: zh-cn\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; "
        ".NET CLR 2.0.50727; TheWorld)\r\n"
        "content-length:28\r\n"
        "Host: www.google.cn\r\n"
        "Connection: Keep-Alive\r\n"
        "Cookie: "
        "PREF=ID=80a06da87be9ae3c:U=f7167333e2c3b714:NW=1:TM=1261551909:LM="
        "1261551917:S=ybYcq2wpfefs4V9g; "
        "NID=31=ojj8d-IygaEtSxLgaJmqSjVhCspkviJrB6omjamNrSm8lZhKy_"
        "yMfO2M4QMRKcH1g0iQv9u\r\n"
        "\r\n"
        "hl=zh-CN&source=hp&q=domety";
    HttpRequest req;
    req.tryDecode(str);


    // for(auto& h : req.getHeaders()) {
    // 	std::cout << "++ key: " << h.first << std::endl;
    // 	std::cout << "   value: " << h.second << std::endl;
    // }
    req.process_header();
    HttpResponse res(req, 0,(char*)"");
    string t("cefgnweurigtnlwr4tuhljer5y");
    res.setBody(t);
    cout << res.getResponseStr() << endl;
    return 0;
}
