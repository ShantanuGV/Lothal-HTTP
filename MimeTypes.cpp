#include "MimeTypes.h"

#include <unordered_map>
using namespace std;

string MimeTypes::get(const string& filePath)
{
    static const unordered_map<string, string> mimeTypes =
    {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".svg", "image/svg+xml"},
        {".ico", "image/x-icon"},
        {".json", "application/json"},
        {".txt", "text/plain"}
    };

    size_t dot = filePath.find_last_of('.');

    if (dot == string::npos)
    {
        return "application/octet-stream";
    }

    string extension = filePath.substr(dot);

    auto it = mimeTypes.find(extension);

    if (it != mimeTypes.end())
    {
        return it->second;
    }

    return "application/octet-stream";
}