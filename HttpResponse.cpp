#include "HttpResponse.h"
using namespace std;

void HttpResponse::setHeader(
    const string& key,
    const string& value)
{
    headers[key] = value;
}

string HttpResponse::getHeader(
    const string& key) const
{
    auto it = headers.find(key);

    if(it == headers.end())
        return "";

    return it->second;
}

const unordered_map<string,string>&
HttpResponse::getHeaders() const
{
    return headers;
}

HttpResponse::HttpResponse()
{
    status = "200 OK";

    headers["Content-Type"] = "text/plain";

    headers["Connection"] = "close";
}

void HttpResponse::setStatus(const string& status)
{
    this->status = status;
}

void HttpResponse::setContentType(
    const string& type)
{
    setHeader("Content-Type", type);
}

void HttpResponse::setBody(const string& body)
{
    this->body = body;
}

string HttpResponse::build() const
{
    string response;

    response += "HTTP/1.1 ";
    response += status;
    response += "\r\n";

    // Write all headers
    for(const auto& header : headers)
    {
        response += header.first;
        response += ": ";
        response += header.second;
        response += "\r\n";
    }

    // Add Content-Length if it wasn't set manually
    if(headers.find("Content-Length") == headers.end())
    {
        response += "Content-Length: ";
        response += to_string(body.size());
        response += "\r\n";
    }

    // Add Connection if it wasn't set manually
    if(headers.find("Connection") == headers.end())
    {
        response += "Connection: close\r\n";
    }

    response += "\r\n";
    response += body;

    return response;
}