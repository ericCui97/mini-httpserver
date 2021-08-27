#ifndef CANDY_HTTP_H
#define CANDY_HTTP_H
#include <map>
#include <string>
#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"
#define CRLFCRLF "\r\n\r\n"
#include <iostream>
#include <string>
#include <vector>
#include "../file_service/file_service.h"
#include "../socket/set_socket.h"
using std::cout;
using std::endl;
using std::map;
using std::string;


#define HTML_TYPE 1
#define SOCK_STREAM 2
struct HttpResponse;
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
    bool isKeepAlive() { return is_keep_alive; }
    void setKeepAlive(bool b) { is_keep_alive = b; }
    void setFD(int fd) { this->_fd = fd; }
    int getFD() { return this->_fd; }

private:
    void parseInternal(const char *buf, int size);

private:
    int _fd;
    string _method;  //请求方法
    bool is_keep_alive = false;
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
    friend void process_header(HttpRequest &req, HttpResponse &res);
};


#define CONTENTTYPE "Content-Type"
#define CONTENT_DISPOSTION "Content-Disposition"
#define HTML_CONTENT_TYPE "text/html"
#define STREAM_CONTENT_TYPE "application/octet-stream"
#define CONTENT_DISPOSTION_VALUE "attachment;filename=xx.xxx"
#define CONTENT_LENGTH "Content-Length"
#define CONTENT_RANGE "Content-Range"

enum http_status {
    HTTP_OK = 200,
    HTTP_NOT_MODIFIED = 304,
    HTTP_NOT_FOUND = 404,
};


//###################### response  ###########################//
class HttpResponse
{
    // TODO:response builder
    // httpResponse    FileService<委托>
private:
    // FileService *f_service;// pimpl composition by reference
    string _protocol;
    string _version;
    string _status;
    int _status_code = 200;
    map<string, string> _res_headers;
    string _body;
    string response;
    string _url;
    char *filename = "index.html";
    int start_pos = 0;
    int end_pos = -1;
    // type: 1:file 2:html
    int type;
    bool isChunked = false;

    int _fd;
    bool is_range = false;

public:
    HttpResponse(HttpRequest &req, uint16_t type, char *filename);
    HttpResponse &addHeader(const string &header_name,
                            const string &header_value);
    HttpResponse &setBody(string &_body);
    string getResponseStr();
    string getHeaderStr();
    void setStCode(int code);
    void setChunked() { this->isChunked = true; };
    bool IsChunked() { return this->isChunked; };
    bool IsRange() { return is_range; };
    void setFileService();
    friend void process_header(HttpRequest &req, HttpResponse &res);
    void send();
};
// class File_Service
// {
// };
static string mapCode2Status(int code)
{
    switch (code) {
    case 200:
        return "OK";
        break;
    case 206:
        return "Partial Content";
        break;
    case 302:
        return "Found";
        break;
    }
    return "";
}
#endif
