#include "CorsMiddleware.h"

using namespace std;

CorsMiddleware::CorsMiddleware()
{
}

void CorsMiddleware::handle(
    HttpRequest& request,
    HttpResponse& response,
    Next next)
{
    response.setHeader(
        "Access-Control-Allow-Origin",
        "*"
    );

    response.setHeader(
        "Access-Control-Allow-Methods",
        "GET, POST, PUT, PATCH, DELETE, OPTIONS"
    );

    response.setHeader(
        "Access-Control-Allow-Headers",
        "Content-Type, Authorization"
    );

    // Handle browser pre-flight request
    if(request.getMethod() == HttpMethod::OPTIONS)
    {
        response.setStatus("204 No Content");
        response.setBody("");
        return;
    }

    next();
}