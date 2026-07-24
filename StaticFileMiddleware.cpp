#include <iostream>

#include "StaticFileMiddleware.h"
#include "HttpRange.h"
#include "HttpRangeResolver.h"
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

    string rangeHeader =
    request.getHeader("Range");

HttpRange range =
    HttpRangeParser::parse(rangeHeader);

long long fileSize =
    FileSystem::getFileSize(file);

if(range.valid)
{
    if(!HttpRangeResolver::resolve(range, fileSize))
    {
        response.setStatus(
            "416 Range Not Satisfiable"
        );

        response.setHeader(
            "Content-Range",
            "bytes */" +
            to_string(fileSize)
        );

        response.setContentType(
            "text/plain"
        );

        response.setBody(
            "Invalid Range"
        );

        return;
    }

    cout << "\n===== RANGE =====\n";

    cout << "Start : "
         << range.start
         << endl;

    cout << "End   : "
         << range.end
         << endl;
}

    string body;

if(range.valid)
{
    body =
        FileSystem::readRange(
            file,
            range.start,
            range.end - range.start + 1
        );

    response.setStatus(
        "206 Partial Content"
    );

    response.setHeader(
        "Accept-Ranges",
        "bytes"
    );

    response.setHeader(
        "Content-Range",
        "bytes " +
        to_string(range.start) +
        "-" +
        to_string(range.end) +
        "/" +
        to_string(fileSize)
    );
}
else
{
    body =
        FileSystem::readFile(file);
}

    cout << "Body Size = "
     << body.size()
     << endl;

    if(body.empty())
    {
        next();
        return;
    }

    if(!range.valid)
{
    response.setStatus("200 OK");
}

response.setHeader(
    "Accept-Ranges",
    "bytes"
);

response.setContentType(
    MimeTypes::get(file));

if(request.getMethod() != HttpMethod::HEAD)
{
    response.setBody(body);
}
}

bool StaticFileMiddleware::isStaticRequest(
    const string& path) const
{
    return path.find('.') != string::npos;
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