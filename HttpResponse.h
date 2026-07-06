#pragma once

#include <string>
using namespace std;

class HttpResponse
{
private:

    string status;
    string contentType;
    string body;

public:

    HttpResponse();

    void setStatus(const string& status);

    void setContentType(const string& type);

    void setBody(const string& body);

    string build() const;
};