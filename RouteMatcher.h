#pragma once

#include "route.h"

class RouteMatcher
{
public:

    static bool match(
        const Route& route,
        HttpRequest& request
    );

private:

    static void splitPath(
        const std::string& path,
        std::vector<std::string>& segments
    );
};