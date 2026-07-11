#pragma once

#include "Middleware.h"

class CorsMiddleware : public Middleware
{
public:

    CorsMiddleware();

    void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) override;
};