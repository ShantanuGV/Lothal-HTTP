#include "HttpRange.h"

using namespace std;

HttpRange HttpRangeParser::parse(
    const string& header)
{
    HttpRange range;

    if(header.empty())
    {
        return range;
    }

    const string prefix = "bytes=";

    if(header.substr(0, prefix.size()) != prefix)
    {
        return range;
    }

    string value =
        header.substr(prefix.size());

    size_t dash =
        value.find('-');

    if(dash == string::npos)
    {
        return range;
    }

    string left =
        value.substr(0, dash);

    string right =
        value.substr(dash + 1);

    try
    {
        // bytes=-500
        if(left.empty())
        {
            range.valid = true;

            range.suffix = true;

            range.end =
                stoll(right);

            return range;
        }

        // bytes=100-
        if(right.empty())
        {
            range.valid = true;

            range.start =
                stoll(left);

            range.hasEnd = false;

            return range;
        }

        // bytes=100-500
        range.valid = true;

        range.start =
            stoll(left);

        range.end =
            stoll(right);

        range.hasEnd = true;

        if(range.end < range.start)
        {
            range.valid = false;
        }
    }
    catch(...)
    {
        range.valid = false;
    }

    return range;
}