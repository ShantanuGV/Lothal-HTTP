#include "RouteCompiler.h"

using namespace std;

void RouteCompiler::compile(Route& route)
{
    route.segments.clear();

    string current;

    for(char c : route.originalPattern)
    {
        if(c == '/')
        {
            if(!current.empty())
            {
                compileSegment(current, route);
                current.clear();
            }
        }
        else
        {
            current += c;
        }

        route.priority = 0;

for(const auto& segment : route.segments)
{
    switch(segment.type)
    {
        case SegmentType::STATIC:
            route.priority += 100;
            break;

        case SegmentType::REGEX:
            route.priority += 80;
            break;

        case SegmentType::PARAMETER:
            route.priority += 60;
            break;

        case SegmentType::OPTIONAL:
            route.priority += 40;
            break;

        case SegmentType::WILDCARD:
            route.priority += 10;
            break;
    }
}
    }

    if(!current.empty())
    {
        compileSegment(current, route);
    }
}

void RouteCompiler::compileSegment(
    const string& text,
    Route& route)
{
    RouteSegment segment;

    //----------------------------------------------------
    // Wildcard
    //----------------------------------------------------

    if(text == "*")
    {
        segment.type = SegmentType::WILDCARD;
    }

    //----------------------------------------------------
    // Parameter
    //----------------------------------------------------

    else if(text[0] == ':')
    {
        string value = text.substr(1);

        //------------------------------------------------
        // Optional parameter
        //------------------------------------------------

        if(!value.empty() &&
           value.back() == '?')
        {
            value.pop_back();

            segment.type =
                SegmentType::OPTIONAL;

            segment.text = value;
        }

        //------------------------------------------------
        // Regex parameter
        //------------------------------------------------

        else
        {
            size_t open =
                value.find('(');

            size_t close =
                value.rfind(')');

            if(open != string::npos &&
               close != string::npos &&
               close > open)
            {
                segment.type =
                    SegmentType::REGEX;

                segment.text =
                    value.substr(0, open);

                string pattern =
                    value.substr(
                        open + 1,
                        close - open - 1
                    );

                segment.expression =
                    regex(pattern);
            }
            else
            {
                segment.type =
                    SegmentType::PARAMETER;

                segment.text = value;
            }
        }
    }

    //----------------------------------------------------
    // Static
    //----------------------------------------------------

    else
    {
        segment.type = SegmentType::STATIC;
        segment.text = text;
    }

    route.segments.push_back(segment);
}