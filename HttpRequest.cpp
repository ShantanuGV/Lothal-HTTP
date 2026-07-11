#include "HttpRequest.h"

#include <iostream>
#include <sstream>
using namespace std;

HttpRequest::HttpRequest()
{
    method = HttpMethod::UNKNOWN;
    path = "";
    version = "";
    body = "";
}

void HttpRequest::setClientIP(
    const string& ip)
{
    clientIP = ip;
}

string HttpRequest::getClientIP() const
{
    return clientIP;
}

bool HttpRequest::parse(const string& rawRequest)
{
    headers.clear();
    query.clear();
    body.clear();

    stringstream stream(rawRequest);

    string requestLine;

    if (!getline(stream, requestLine))
    {
        return false;
    }

    if (!requestLine.empty() && requestLine.back() == '\r')
    {
        requestLine.pop_back();
    }

    if (!parseRequestLine(requestLine))
    {
        return false;
    }

    parseHeaders(stream);

    parseQuery();

    // Remaining data is the body
    string line;

    while (getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        body += line;

        if (!stream.eof())
        {
            body += '\n';
        }
    }

    return true;
}

bool HttpRequest::parseRequestLine(const string& line)
{
    stringstream ss(line);

    string methodText;

    if (!(ss >> methodText >> path >> version))
    {
        return false;
    }

    method = parseMethod(methodText);

    return true;
}

void HttpRequest::parseHeaders(stringstream& stream)
{
    string line;

    while (getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        // Empty line = end of headers
        if (line.empty())
        {
            break;
        }

        size_t colon = line.find(':');

        if (colon == string::npos)
        {
            continue;
        }

        string key = line.substr(0, colon);

        string value = line.substr(colon + 1);

        // Remove leading space
        if (!value.empty() && value[0] == ' ')
        {
            value.erase(0, 1);
        }

        headers[key] = value;
    }
}

void HttpRequest::parseQuery()
{
    size_t questionMark = path.find('?');

    if (questionMark == string::npos)
    {
        return;
    }

    string queryString = path.substr(questionMark + 1);

    path = path.substr(0, questionMark);

    stringstream ss(queryString);

    string pair;

    while (getline(ss, pair, '&'))
    {
        size_t equal = pair.find('=');

        if (equal == string::npos)
        {
            continue;
        }

        string key = pair.substr(0, equal);

        string value = pair.substr(equal + 1);

        query[key] = value;
    }
}

HttpMethod HttpRequest::parseMethod(const string& text)
{
    if (text == "GET") return HttpMethod::GET;
    if (text == "POST") return HttpMethod::POST;
    if (text == "PUT") return HttpMethod::PUT;
    if (text == "PATCH") return HttpMethod::PATCH;
    if (text == "DELETE") return HttpMethod::DELETE_;
    if (text == "HEAD") return HttpMethod::HEAD;
    if (text == "OPTIONS") return HttpMethod::OPTIONS;
    if (text == "CONNECT") return HttpMethod::CONNECT;
    if (text == "TRACE") return HttpMethod::TRACE;
    if (text == "QUERY") return HttpMethod::QUERY;

    return HttpMethod::UNKNOWN;
}

HttpMethod HttpRequest::getMethod() const
{
    return method;
}

string HttpRequest::getPath() const
{
    return path;
}

string HttpRequest::getVersion() const
{
    return version;
}

string HttpRequest::getHeader(const string& key) const
{
    auto it = headers.find(key);

    if (it == headers.end())
    {
        return "";
    }

    return it->second;
}

string HttpRequest::getQuery(const string& key) const
{
    auto it = query.find(key);

    if (it == query.end())
    {
        return "";
    }

    return it->second;
}

string HttpRequest::getBody() const
{
    return body;
}

string HttpRequest::getParam(const string& key) const
{
    auto it = params.find(key);

    if(it == params.end())
        return "";

    return it->second;
}

void HttpRequest::setParam(
    const string& key,
    const string& value)
{
    params[key] = value;
}

ostream& operator<<(ostream& os, HttpMethod method)
{
    switch (method)
    {
    case HttpMethod::GET:      os << "GET"; break;
    case HttpMethod::POST:     os << "POST"; break;
    case HttpMethod::PUT:      os << "PUT"; break;
    case HttpMethod::PATCH:    os << "PATCH"; break;
    case HttpMethod::DELETE_:  os << "DELETE"; break;
    case HttpMethod::HEAD:     os << "HEAD"; break;
    case HttpMethod::OPTIONS:  os << "OPTIONS"; break;
    case HttpMethod::CONNECT:  os << "CONNECT"; break;
    case HttpMethod::TRACE:    os << "TRACE"; break;
    case HttpMethod::QUERY:    os << "QUERY"; break;
    default:                   os << "UNKNOWN"; break;
    }

    return os;
}