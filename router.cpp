#include "router.h"

#include <algorithm>

#include "FileSystem.h"
#include "MimeTypes.h"
#include "RouteCompiler.h"
#include "RouteMatcher.h"
#include "RouteSorter.h"

using namespace std;

Router::Router()
{
}

void Router::addRoute(
    HttpMethod method,
    const string& path,
    Handler handler)
{
    Route route;

route.method = method;
route.originalPattern = path;
route.handler = handler;

RouteCompiler::compile(route);

routes[method].push_back(route);

RouteSorter::sort(routes[method]);
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

vector<string> Router::splitPath(
    const string& path) const
{
    vector<string> segments;

    string current;

    for(char c : path)
    {
        if(c == '/')
        {
            if(!current.empty())
            {
                segments.push_back(current);
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }

    if(!current.empty())
    {
        segments.push_back(current);
    }

    return segments;
}

void Router::parsePattern(Route& route)
{
    vector<string> parts = splitPath(route.originalPattern);

    for(const string& part : parts)
    {
        RouteSegment seg;

        //--------------------------------
        // Wildcard
        //--------------------------------

        if(part == "*")
        {
            seg.type = SegmentType::WILDCARD;
        }

        //--------------------------------
        // Parameter
        //--------------------------------

        else if(part[0] == ':')
        {
            seg.type = SegmentType::PARAMETER;

            seg.text = part.substr(1);

            //--------------------------------
            // Optional
            //--------------------------------

            if(!seg.text.empty() &&
               seg.text.back() == '?')
            {
                seg.type = SegmentType::OPTIONAL;

                seg.text.pop_back();
            }

            //--------------------------------
            // Regex
            //--------------------------------

            size_t open = seg.text.find('(');

            size_t close = seg.text.find(')');

            if(open != string::npos &&
               close != string::npos)
            {
                string name =
                    seg.text.substr(0, open);

                string regexText =
                    seg.text.substr(
                        open + 1,
                        close - open - 1);

                seg.text = name;

                seg.type = SegmentType::REGEX;

                seg.expression =
                    regex(regexText);
            }
        }

        //--------------------------------
        // Static
        //--------------------------------

        else
        {
            seg.type = SegmentType::STATIC;

            seg.text = part;
        }

        route.segments.push_back(seg);
    }
}

int Router::calculatePriority(
    const Route& route)
{
    int priority = 0;

    for(const RouteSegment& seg : route.segments)
    {
        switch(seg.type)
        {
            case SegmentType::STATIC:

                priority += 100;

                break;

            case SegmentType::REGEX:

                priority += 80;

                break;

            case SegmentType::PARAMETER:

                priority += 60;

                break;

            case SegmentType::OPTIONAL:

                priority += 40;

                break;

            case SegmentType::WILDCARD:

                priority += 10;

                break;
        }
    }

    return priority;
}

bool Router::handle(
    HttpRequest& request,
    HttpResponse& response)
{
    auto methodIt =
        routes.find(request.getMethod());

    if(methodIt == routes.end())
    {
        return serveStaticFile(
            request,
            response);
    }

    for(const Route& route : methodIt->second)
    {
        if(RouteMatcher::match(route, request))
        {
            route.handler(request, response);
            return true;
        }
    }

    return serveStaticFile(
        request,
        response);
}

bool Router::serveStaticFile(
    HttpRequest& request,
    HttpResponse& response)
{
    string path = request.getPath();

    if(path == "/")
    {
        path = "/index.html";
    }

    string file = "public" + path;

    string body = FileSystem::readFile(file);

    if(body.empty())
    {
        response.setStatus("404 Not Found");
        response.setContentType("text/html");
        response.setBody("<h1>404 Not Found</h1>");

        return false;
    }

    response.setStatus("200 OK");
    response.setContentType(MimeTypes::get(file));
    response.setBody(body);

    return true;
}