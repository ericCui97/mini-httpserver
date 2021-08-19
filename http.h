#ifndef CANDY_HTTP_H
#define CANDY_HTTP_H


#include <map>
#include <string>
#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"
#define CRLFCRLF "\r\n\r\n"
#include "set_socket.h"


using std::map;
using std::string;

/**
 * 字符串的buf，只存储对应的指针，不存储实际的内容
 */
struct StringBuffer {
    char *begin = NULL;  //字符串开始位置
    char *end = NULL;    //字符串结束位置

    operator string() const { return string(begin, end); }
};

/**
 * Http请求行的状态
 */
enum class HttpRequestDecodeState {
    INVALID,          //无效
    INVALID_METHOD,   //无效请求方法
    INVALID_URI,      //无效的请求路径
    INVALID_VERSION,  //无效的协议版本号
    INVALID_HEADER,   //无效请求头


    START,   //请求行开始
    METHOD,  //请求方法

    BEFORE_URI,              //请求连接前的状态，需要'/'开头
    IN_URI,                  // url处理
    BEFORE_URI_PARAM_KEY,    // URL请求参数键之前
    URI_PARAM_KEY,           // URL请求参数键
    BEFORE_URI_PARAM_VALUE,  // URL的参数值之前
    URI_PARAM_VALUE,         // URL请求参数值

    BEFORE_PROTOCOL,  //协议解析之前
    PROTOCOL,         //协议

    BEFORE_VERSION,  //版本开始前
    VERSION_SPLIT,   //版本分隔符 '.'
    VERSION,         //版本
    HEADER_KEY,

    HEADER_BEFORE_COLON,  //冒号之前
    HEADER_AFTER_COLON,   //冒号
    HEADER_VALUE,         //值

    WHEN_CR,  //遇到一个回车之后

    CR_LF,  //回车换行

    CR_LF_CR,  //回车换行之后的状态


    BODY,  //请求体

    COMPLETE,  //完成
};
/**
 * http的请求类
 */


struct HttpRequest {
    /**
     * 解析http协议
     * @param buf
     * @return
     */
    void tryDecode(const std::string &buf);

    const std::string &getMethod() const;

    const std::string &getUrl() const;

    const std::map<std::string, std::string> &getRequestParams() const;

    const std::string &getProtocol() const;

    const std::string &getVersion() const;

    const std::map<std::string, std::string> &getHeaders() const;

    const std::string &getBody() const;


private:
    void parseInternal(const char *buf, int size);

private:
    string _method;  //请求方法

    string _url;  //请求路径[不包含请求参数]

    map<string, string> _requestParams;  //请求参数

    string _protocol;  //协议
    string _version;   //版本

    map<string, string> _headers;  //所有的请求头

    string _body;  //请求体

    int _nextPos = 0;  //下一个位置的

    HttpRequestDecodeState _decodeState =
        HttpRequestDecodeState::START;  //解析状态
public:
    int process_header(int);
};


#define CONTENTTYPE "Content-Type"
#define CONTENT_DISPOSTION "Content-Disposition"
#define HTML_CONTENT_TYPE "text/html"
#define STREAM_CONTENT_TYPE "application/octet-stream"
#define CONTENT_DISPOSTION_VALUE "attachment;filename=xx.xxx"


enum http_status {
    HTTP_OK = 200,
    HTTP_NOT_MODIFIED = 304,
    HTTP_NOT_FOUND = 404,
};
class HttpResponse
{
    // TODO:response builder
private:
    string _protocol;
    string _version;
    http_status _status;
    map<string, string> _res_headers;
    string _body;
    string response;
    string _url;

public:
    HttpResponse(const HttpRequest &req, uint16_t flag);
    ~HttpResponse();
    HttpResponse &addHeader(const string &header_name,
                            const string &header_value);
    HttpResponse &setBody(string &_body);
    string getResponseStr();
};
// class File_Service
// {
// };



#endif
