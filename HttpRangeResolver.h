#pragma once

#include "HttpRange.h"

class HttpRangeResolver
{
public:

    static bool resolve(
        HttpRange& range,
        long long fileSize
    );
};