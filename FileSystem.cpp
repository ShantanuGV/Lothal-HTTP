#include "FileSystem.h"
#include <iostream>

#include <fstream>
#include <sstream>
using namespace std;


string FileSystem::readRange(
    const string& path,
    long long start,
    long long length)
{
    ifstream file(path, ios::binary);

    if(!file.is_open())
    {
        return "";
    }

    file.seekg(start);

    string buffer(length, '\0');

    file.read(&buffer[0], length);

    buffer.resize(file.gcount());

    return buffer;
}

long long FileSystem::getFileSize(
    const string& path)
{
    ifstream file(path, ios::binary | ios::ate);

    if(!file.is_open())
    {
        return -1;
    }

    return file.tellg();
}

string FileSystem::readFile(const string& path)
{
    ifstream file(path, ios::binary);

    if (!file.is_open())
    {
        return "";
    }

    stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}