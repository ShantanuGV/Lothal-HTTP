#pragma once

#include <functional>
#include <regex>
#include <string>
#include <vector>

#include "HttpRequest.h"
#include "HttpResponse.h"

using Handler = std::function<void(HttpRequest&, HttpResponse&)>;

enum class SegmentType
{
    STATIC,
    PARAMETER,
    WILDCARD,
    REGEX,
    OPTIONAL
};

struct RouteSegment
{
    SegmentType type = SegmentType::STATIC;

    std::string text;

    std::regex expression;
};

struct Route
{
    HttpMethod method;

    // Route registered by the user
    std::string originalPattern;

    // Compiled route segments
    std::vector<RouteSegment> segments;

    Handler handler;

    int priority = 0;
};