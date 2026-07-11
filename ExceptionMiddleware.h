#pragma once

#include "Middleware.h"

class ExceptionMiddleware : public Middleware
{
public:

    ExceptionMiddleware();

    void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) override;
};