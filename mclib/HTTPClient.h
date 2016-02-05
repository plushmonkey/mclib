#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include <map>
#include <json/json.h>
#include "mclib.h"

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
    MCLIB_API CurlHTTPClient();
    MCLIB_API ~CurlHTTPClient();
    HTTPResponse MCLIB_API Get(const std::string& url, Headers headers = {});
    HTTPResponse MCLIB_API Post(const std::string& url, const std::string& data, Headers headers = {});
    HTTPResponse MCLIB_API PostJSON(const std::string& url, const std::string& data, Headers headers = {});
    HTTPResponse MCLIB_API PostJSON(const std::string& url, const Json::Value& json, Headers headers = {});
};

#endif
