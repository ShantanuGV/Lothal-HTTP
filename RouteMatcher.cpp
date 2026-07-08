#include "RouteMatcher.h"

using namespace std;

void RouteMatcher::splitPath(
    const string& path,
    vector<string>& segments)
{
    segments.clear();

    string current;

    for(char c : path)
    {
        if(c == '/')
        {
            if(!current.empty())
            {
                segments.push_back(current);
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }

    if(!current.empty())
    {
        segments.push_back(current);
    }
}

bool RouteMatcher::match(
    const Route& route,
    HttpRequest& request)
{
    vector<string> url;

    splitPath(
        request.getPath(),
        url
    );

    size_t i = 0;
    size_t j = 0;

    while(i < route.segments.size())
    {
        const RouteSegment& segment =
            route.segments[i];

        //-----------------------------------
        // Wildcard
        //-----------------------------------

        if(segment.type == SegmentType::WILDCARD)
        {
            return true;
        }

        //-----------------------------------
        // Optional
        //-----------------------------------

        if(segment.type == SegmentType::OPTIONAL)
        {
            if(j < url.size())
            {
                request.setParam(
                    segment.text,
                    url[j]
                );

                j++;
            }

            i++;
            continue;
        }

        //-----------------------------------
        // URL shorter than route
        //-----------------------------------

        if(j >= url.size())
        {
            return false;
        }

        //-----------------------------------
        // Static
        //-----------------------------------

        if(segment.type == SegmentType::STATIC)
        {
            if(segment.text != url[j])
            {
                return false;
            }
        }

        //-----------------------------------
        // Parameter
        //-----------------------------------

        else if(segment.type ==
                SegmentType::PARAMETER)
        {
            request.setParam(
                segment.text,
                url[j]
            );
        }

        //-----------------------------------
        // Regex Parameter
        //-----------------------------------

        else if(segment.type ==
                SegmentType::REGEX)
        {
            if(!regex_match(
                    url[j],
                    segment.expression))
            {
                return false;
            }

            request.setParam(
                segment.text,
                url[j]
            );
        }

        i++;
        j++;
    }

    return j == url.size();
}