#pragma once

#include <vector>

#include "route.h"

class RouteSorter
{
public:

    static void sort(std::vector<Route>& routes);

private:

    static bool compare(
        const Route& a,
        const Route& b);
};