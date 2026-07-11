#pragma once

#include <functional>

#include "HttpRequest.h"
#include "HttpResponse.h"

using Next = std::function<void()>;

class Middleware
{
public:

    virtual ~Middleware() = default;

    virtual void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) = 0;
};