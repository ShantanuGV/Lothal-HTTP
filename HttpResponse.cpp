#include "HttpResponse.h"
using namespace std;

HttpResponse::HttpResponse()
{
    status = "200 OK";
    contentType = "text/plain";
    body = "";
}

void HttpResponse::setStatus(const string& status)
{
    this->status = status;
}

void HttpResponse::setContentType(const string& type)
{
    this->contentType = type;
}

void HttpResponse::setBody(const string& body)
{
    this->body = body;
}

string HttpResponse::build() const
{
    return
        "HTTP/1.1 " + status + "\r\n"
        "Content-Type: " + contentType + "\r\n"
        "Content-Length: " + to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        body;
}