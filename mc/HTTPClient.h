#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include <map>
#include <json/json.h>

typedef std::map<std::string, std::string> Headers;

struct HTTPResponse {
    int status;
    Headers headers;
    std::string body;
};

class HTTPClient {
public:
    virtual ~HTTPClient() { }

    virtual HTTPResponse Get(const std::string& url, Headers headers = {}) = 0;
    virtual HTTPResponse Post(const std::string& url, const std::string& data, Headers headers = {}) = 0;
    virtual HTTPResponse PostJSON(const std::string& url, const std::string& data, Headers headers = {}) = 0;
    virtual HTTPResponse PostJSON(const std::string& url, const Json::Value& json, Headers headers = {}) = 0;
};

class CurlHTTPClient : public HTTPClient {
private:
    class Impl;
    Impl* m_Impl;
public:
    CurlHTTPClient();
    ~CurlHTTPClient();
    HTTPResponse Get(const std::string& url, Headers headers = {});
    HTTPResponse Post(const std::string& url, const std::string& data, Headers headers = {});
    HTTPResponse PostJSON(const std::string& url, const std::string& data, Headers headers = {});
    HTTPResponse PostJSON(const std::string& url, const Json::Value& json, Headers headers = {});
};

#endif
