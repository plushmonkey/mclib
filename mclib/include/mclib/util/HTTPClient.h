#ifndef MCLIB_UTIL_HTTP_CLIENT_H_
#define MCLIB_UTIL_HTTP_CLIENT_H_

#include <mclib/common/JsonFwd.h>
#include <mclib/mclib.h>

#include <map>
#include <memory>

namespace mc {
namespace util {

typedef std::map<std::string, std::string> Headers;

struct HTTPResponse {
    int status;
    Headers headers;
    std::string body;
};

class HTTPClient {
public:
    HTTPClient() = default;
    virtual ~HTTPClient() {}
    HTTPClient(HTTPClient&& other) = default;
    HTTPClient& operator=(HTTPClient&& rhs) = default;

    virtual HTTPResponse Get(const std::string& url, Headers headers = {}) = 0;
    virtual HTTPResponse Post(const std::string& url, const std::string& data,
                              Headers headers = {}) = 0;
    virtual HTTPResponse PostJSON(const std::string& url,
                                  const std::string& data,
                                  Headers headers = {}) = 0;
    virtual HTTPResponse PostJSON(const std::string& url, const json& json,
                                  Headers headers = {}) = 0;
};

class CurlHTTPClient : public HTTPClient {
private:
    class Impl;
    std::unique_ptr<Impl> m_Impl;

public:
    MCLIB_API CurlHTTPClient();
    MCLIB_API ~CurlHTTPClient();

    CurlHTTPClient(const CurlHTTPClient& other);
    CurlHTTPClient& operator=(const CurlHTTPClient& rhs);
    CurlHTTPClient(CurlHTTPClient&& other);
    CurlHTTPClient& operator=(CurlHTTPClient&& rhs);

    HTTPResponse MCLIB_API Get(const std::string& url, Headers headers = {});
    HTTPResponse MCLIB_API Post(const std::string& url, const std::string& data,
                                Headers headers = {});
    HTTPResponse MCLIB_API PostJSON(const std::string& url,
                                    const std::string& data,
                                    Headers headers = {});
    HTTPResponse MCLIB_API PostJSON(const std::string& url, const json& json,
                                    Headers headers = {});
};

}  // namespace util
}  // namespace mc

#endif
