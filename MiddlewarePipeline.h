#pragma once

#include <memory>
#include <vector>

#include "Middleware.h"

class MiddlewarePipeline
{
public:

    MiddlewarePipeline();

    void use(std::shared_ptr<Middleware> middleware);

    void use(
    const std::string& prefix,
    std::shared_ptr<Middleware> middleware
);

    void execute(
        HttpRequest& request,
        HttpResponse& response,
        Next finalHandler
    );

private:

    struct MiddlewareEntry
{
    std::string prefix;              // "" means global
    std::shared_ptr<Middleware> middleware;
};

    std::vector<MiddlewareEntry> middlewares;

    void executeMiddleware(
        size_t index,
        HttpRequest& request,
        HttpResponse& response,
        Next finalHandler
    );

    
};