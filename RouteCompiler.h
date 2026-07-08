#pragma once

#include "route.h"

class RouteCompiler
{
public:

    static void compile(Route& route);

private:

    static void compileSegment(
        const std::string& text,
        Route& route
    );
};