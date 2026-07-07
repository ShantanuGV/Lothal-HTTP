#include "Router.h"
#include "FileSystem.h"
#include "MimeTypes.h"

Router::Router()
{
}

void Router::addRoute(
    HttpMethod method,
    const string& path,
    Handler handler)
{
    routes[method][path] = handler;
}

void Router::get(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::GET, path, handler);
}

void Router::post(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::POST, path, handler);
}

void Router::put(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::PUT, path, handler);
}

void Router::patch(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::PATCH, path, handler);
}

void Router::del(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::DELETE_, path, handler);
}

void Router::head(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::HEAD, path, handler);
}

void Router::options(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::OPTIONS, path, handler);
}

void Router::query(
    const string& path,
    Handler handler)
{
    addRoute(HttpMethod::QUERY, path, handler);
}

bool Router::handle(
    HttpRequest& request,
    HttpResponse& response)
{
    auto methodIt = routes.find(request.getMethod());

    if (methodIt != routes.end())
    {
        auto routeIt =
            methodIt->second.find(request.getPath());

        if (routeIt != methodIt->second.end())
        {
            routeIt->second(request, response);
            return true;
        }
    }

    return serveStaticFile(request, response);
}

bool Router::serveStaticFile(
    HttpRequest& request,
    HttpResponse& response)
{
    std::string path = request.getPath();

    if (path == "/")
        path = "/index.html";

    std::string file =
        "public" + path;

    std::string body =
        FileSystem::readFile(file);

    if (body.empty())
    {
        response.setStatus("404 Not Found");
        response.setContentType("text/html");
        response.setBody(
            "<h1>404 Not Found</h1>"
        );

        return false;
    }

    response.setStatus("200 OK");

    response.setContentType(
        MimeTypes::get(file));

    response.setBody(body);

    return true;
}