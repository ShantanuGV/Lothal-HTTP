#pragma once

#include <ostream>
#include <string>
#include <unordered_map>
using namespace std;

enum class HttpMethod
{
    GET,
    POST,
    PUT,
    PATCH,
    DELETE_,
    HEAD,
    OPTIONS,
    CONNECT,
    TRACE,
    QUERY,
    UNKNOWN
};

class HttpRequest
{
private:

    HttpMethod method;

    string path;

    string version;

    unordered_map<string, string> headers;

    unordered_map<string, string> params;

    unordered_map<string, string> query;

    string clientIP;

    string body;

    bool parseRequestLine(const string& line);

    void parseHeaders(stringstream& stream);

    void parseQuery();

    HttpMethod parseMethod(const string& text);

public:

    HttpRequest();

    bool parse(const string& rawRequest);

    HttpMethod getMethod() const;

    string getPath() const;

    string getVersion() const;

    string getHeader(const string& key) const;

    string getQuery(const string& key) const;

    string getBody() const;

    string getParam(const string& key) const;

    void setParam(const string& key,
                  const string& value);

    void setClientIP(const string& ip);

    string getClientIP() const;              
};

ostream& operator<<(ostream& os, HttpMethod method);