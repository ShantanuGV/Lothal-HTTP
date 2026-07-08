#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "route.h"

class Router
{
public:

    using Handler = std::function<void(HttpRequest&, HttpResponse&)>;

    Router();

    //-------------------------
    // Route Registration
    //-------------------------

    void get(const std::string& path, Handler handler);

    void post(const std::string& path, Handler handler);

    void put(const std::string& path, Handler handler);

    void patch(const std::string& path, Handler handler);

    void del(const std::string& path, Handler handler);

    void head(const std::string& path, Handler handler);

    void options(const std::string& path, Handler handler);

    void query(const std::string& path, Handler handler);

    //-------------------------
    // Route Dispatcher
    //-------------------------

    bool handle(HttpRequest& request,
                HttpResponse& response);

private:

    //-------------------------------------------------
    // One routing table per HTTP method.
    //
    // GET
    //   ├── /about
    //   ├── /users/:id
    //   ├── /files/*
    //
    // POST
    //   ├── /login
    //   ├── /register
    //-------------------------------------------------

    std::unordered_map<
        HttpMethod,
        std::vector<Route>
    > routes;

    //-------------------------
    // Internal Helpers
    //-------------------------

    void addRoute(HttpMethod method,
                  const std::string& path,
                  Handler handler);

    void parsePattern(Route& route);

    int calculatePriority(const Route& route);

    bool matchRoute(const Route& route,
                    HttpRequest& request);

    std::vector<std::string>
    splitPath(const std::string& path) const;

    bool serveStaticFile(HttpRequest& request,
                         HttpResponse& response);
};