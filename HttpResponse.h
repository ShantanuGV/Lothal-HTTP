#pragma once

#include <string>
#include <unordered_map>
using namespace std;

class HttpResponse
{
private:

    string status;
    string contentType;
    string body;
    std::unordered_map<std::string, std::string> headers;

public:

    HttpResponse();

    void setStatus(const string& status);

    void setContentType(const string& type);

    void setBody(const string& body);

    string build() const;

    void setHeader(const std::string& key, const std::string& value);

    std::string getHeader(const std::string& key) const;

    const std::unordered_map<std::string,std::string>&
    getHeaders() const;
};