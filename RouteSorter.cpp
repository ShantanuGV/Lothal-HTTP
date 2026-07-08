#include "RouteSorter.h"

#include <algorithm>

using namespace std;

bool RouteSorter::compare(
    const Route& a,
    const Route& b)
{
    //------------------------------------
    // Higher priority wins
    //------------------------------------

    if(a.priority != b.priority)
    {
        return a.priority > b.priority;
    }

    //------------------------------------
    // Longer route wins
    //------------------------------------

    if(a.segments.size() != b.segments.size())
    {
        return a.segments.size() >
               b.segments.size();
    }

    //------------------------------------
    // Tie breaker
    //------------------------------------

    return a.originalPattern <
           b.originalPattern;
}

void RouteSorter::sort(
    vector<Route>& routes)
{
    std::sort(
        routes.begin(),
        routes.end(),
        compare
    );
}