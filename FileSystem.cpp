#include "FileSystem.h"
#include <iostream>

#include <fstream>
#include <sstream>
using namespace std;

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