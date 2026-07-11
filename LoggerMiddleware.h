#pragma once

#include "Middleware.h"

class LoggerMiddleware : public Middleware
{
public:

    LoggerMiddleware();

    void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) override;
};