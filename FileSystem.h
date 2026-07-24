#pragma once

#include <string>
using namespace std;

class FileSystem
{
public:

    static string readFile(const string& path);

    static long long getFileSize(const std::string& path);

    static std::string readRange(const std::string& path, long long start, long long length);
};