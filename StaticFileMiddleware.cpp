#include <iostream>

#include "StaticFileMiddleware.h"

#include "FileSystem.h"
#include "MimeTypes.h"

using namespace std;

StaticFileMiddleware::StaticFileMiddleware(
    const string& root)
{
    rootDirectory = root;
}

void StaticFileMiddleware::handle(
    HttpRequest& request,
    HttpResponse& response,
    Next next)
{
    string path = request.getPath();

    if(path == "/")
    {
        path = "/index.html";
    }

    if(!isSafePath(path))
    {
        response.setStatus("403 Forbidden");
        response.setContentType("text/plain");
        response.setBody("Forbidden");

        return;
    }

    string file = buildPath(path);

    string body = FileSystem::readFile(file);

    cout << "Body Size = "
     << body.size()
     << endl;

    if(body.empty())
    {
        next();
        return;
    }

    response.setStatus("200 OK");

    response.setContentType(
        MimeTypes::get(file));

    response.setBody(body);
}

bool StaticFileMiddleware::isStaticRequest(
    const string& path) const
{
    return true;
}

string StaticFileMiddleware::buildPath(
    const string& path) const
{
    string full = rootDirectory + path;

    cout << "Static file path: "
         << full
         << endl;

    return full;
}

bool StaticFileMiddleware::isSafePath(
    const string& path) const
{
    return path.find("..") == string::npos;
}