#include "MiddlewarePipeline.h"

using namespace std;

MiddlewarePipeline::MiddlewarePipeline()
{
}

void MiddlewarePipeline::use(
    shared_ptr<Middleware> middleware)
{
    middlewares.push_back(
    {
        "",
        middleware
    });
}

void MiddlewarePipeline::use(
    const string& prefix,
    shared_ptr<Middleware> middleware)
{
    middlewares.push_back(
    {
        prefix,
        middleware
    });
}

void MiddlewarePipeline::execute(
    HttpRequest& request,
    HttpResponse& response,
    Next finalHandler)
{
    executeMiddleware(
        0,
        request,
        response,
        finalHandler
    );
}

void MiddlewarePipeline::executeMiddleware(
    size_t index,
    HttpRequest& request,
    HttpResponse& response,
    Next finalHandler)
{
    while(index < middlewares.size())
    {
        MiddlewareEntry& entry = middlewares[index];

        // Run only if:
        // 1. Global middleware (prefix == "")
        // 2. Request path starts with prefix
        if(entry.prefix.empty() ||
           request.getPath().rfind(entry.prefix, 0) == 0)
        {
            entry.middleware->handle(
                request,
                response,
                [this,
                 index,
                 &request,
                 &response,
                 finalHandler]()
                {
                    executeMiddleware(
                        index + 1,
                        request,
                        response,
                        finalHandler
                    );
                });

            return;
        }

        // Skip this middleware
        index++;
    }

    finalHandler();
}