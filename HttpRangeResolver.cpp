#include "HttpRangeResolver.h"

using namespace std;

bool HttpRangeResolver::resolve(
    HttpRange& range,
    long long fileSize)
{
    if(!range.valid)
    {
        return false;
    }

    // bytes=-500
    if(range.suffix)
    {
        if(range.end > fileSize)
        {
            range.end = fileSize;
        }

        range.start = fileSize - range.end;
        range.end = fileSize - 1;

        return true;
    }

    // bytes=500-
    if(!range.hasEnd)
    {
        range.end = fileSize - 1;
    }

    if(range.start >= fileSize)
    {
        return false;
    }

    if(range.end >= fileSize)
    {
        range.end = fileSize - 1;
    }

    if(range.start > range.end)
    {
        return false;
    }

    return true;
}