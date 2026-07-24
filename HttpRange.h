#pragma once

#include <string>

struct HttpRange
{
    bool valid = false;

    long long start = 0;

    long long end = 0;

    bool hasEnd = false;

    bool suffix = false;
};

class HttpRangeParser
{
public:

    static HttpRange parse(
        const std::string& header
    );
};