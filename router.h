#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

class Router
{
public:

    using Handler = function<void(HttpRequest&, HttpResponse&)>;

    Router();

    void get(const string& path, Handler handler);

    void post(const string& path, Handler handler);

    void put(const string& path, Handler handler);

    void patch(const string& path, Handler handler);

    void del(const string& path, Handler handler);

    void head(const string& path, Handler handler);

    void options(const string& path, Handler handler);

    void query(const string& path, Handler handler);

    bool handle(HttpRequest& request,
                HttpResponse& response);

private:

    unordered_map<
        HttpMethod,
        unordered_map<string, Handler>
    > routes;

    void addRoute(HttpMethod method,
                  const string& path,
                  Handler handler);

    bool serveStaticFile(HttpRequest& request, HttpResponse& response);
};