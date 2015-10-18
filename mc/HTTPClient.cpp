#include "HTTPClient.h"

#include "Tokenizer.h"

#include <curl/curl.h>
#include <iostream>
#include <string>

std::size_t CurlWriteString(void* buffer, std::size_t size, std::size_t nmemb, void* result) {
    if (result) {
        std::string* out = static_cast<std::string*>(result);
        out->append(static_cast<char*>(buffer), nmemb);
    }

    return size * nmemb;
}

class CurlHTTPClient::Impl {
private:
    CURL* m_Curl;
    unsigned int m_Timeout = 12000;


    Headers GetResponseHeaders(std::string header) {
        std::size_t endFirst = header.find("\n");

        if (endFirst != std::string::npos)
            header = header.substr(endFirst + 1);

        Tokenizer lines(header);

        lines('\n');

        Headers headers;

        for (auto line : lines) {
            Tokenizer kv(line);
            kv(':', 2);

            if (kv.size() != 2 || kv[0].length() == 0 || kv[1].length() == 0) continue;

            headers[kv[0]] = kv[1].substr(0, kv[1].length() - 1);
        }
        return headers;
    }

    int GetStatus(const std::string& header) {
        std::size_t first = header.find(" ");
        std::size_t second = header.find(" ", first + 1);

        if (first != std::string::npos && second != std::string::npos && second != first) {
            std::string statusStr = header.substr(first + 1, second - first);
            if (statusStr.length() == 0)
                return 0;
            return strtol(statusStr.c_str(), nullptr, 10);
        }

        return 0;
    }

    HTTPResponse DoRequest(const std::string& url, const std::string& postData, Headers headers) {
        curl_slist* header_list = nullptr;
        CURLcode res;

        if (headers.size() > 0) {
            for (auto kv : headers) {
                std::string header = kv.first + ": " + kv.second;
                header_list = curl_slist_append(header_list, header.c_str());
            }
        }

        std::string data, header;

        curl_easy_setopt(m_Curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, CurlWriteString);
        curl_easy_setopt(m_Curl, CURLOPT_HEADERFUNCTION, CurlWriteString);
        curl_easy_setopt(m_Curl, CURLOPT_TIMEOUT_MS, m_Timeout);
        curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(m_Curl, CURLOPT_HEADERDATA, &header);
        curl_easy_setopt(m_Curl, CURLOPT_SSL_VERIFYPEER, FALSE);
        if (postData.length() > 0)
            curl_easy_setopt(m_Curl, CURLOPT_POSTFIELDS, postData.c_str());
        if (header_list)
            curl_easy_setopt(m_Curl, CURLOPT_HTTPHEADER, header_list);

        res = curl_easy_perform(m_Curl);

        HTTPResponse response;

        if (header.length() > 0)
            response.status = GetStatus(header);
        else
            response.status = 0;

        if (res == CURLE_OK) {
            response.headers = GetResponseHeaders(header);
            response.body = data;
        }

        return response;
    }
public:
    Impl() {
        m_Curl = curl_easy_init();
    }

    ~Impl() {
        curl_easy_cleanup(m_Curl);
    }

    HTTPResponse Get(const std::string& url, Headers headers) {
        return DoRequest(url, "", headers);
    }

    HTTPResponse Post(const std::string& url, const std::string& postData, Headers headers) {
        return DoRequest(url, postData, headers);
    }

    HTTPResponse PostJSON(const std::string& url, const std::string& postData, Headers headers) {
        headers["Content-Type"] = "application/json";
        return DoRequest(url, postData, headers);
    }

    HTTPResponse PostJSON(const std::string& url, const Json::Value& json, Headers headers) {
        headers["Content-Type"] = "application/json";

        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";

        std::string postData = Json::writeString(builder, json);

        return DoRequest(url, postData, headers);
    }
};

CurlHTTPClient::CurlHTTPClient()
 : m_Impl(new CurlHTTPClient::Impl()) {

}

CurlHTTPClient::~CurlHTTPClient() {
    delete m_Impl;
}

HTTPResponse CurlHTTPClient::Get(const std::string& url, Headers headers) {
    return m_Impl->Get(url, headers);
}

HTTPResponse CurlHTTPClient::Post(const std::string& url, const std::string& data, Headers headers) {
    return m_Impl->Post(url, data, headers);
}
HTTPResponse CurlHTTPClient::PostJSON(const std::string& url, const std::string& data, Headers headers) {
    return m_Impl->PostJSON(url, data, headers);
}

HTTPResponse CurlHTTPClient::PostJSON(const std::string& url, const Json::Value& json, Headers headers) {
    return m_Impl->PostJSON(url, json, headers);
}