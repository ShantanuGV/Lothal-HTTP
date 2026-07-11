#include "AuthMiddleware.h"

using namespace std;

AuthMiddleware::AuthMiddleware()
{
    publicRoutes.insert("/");

    publicRoutes.insert("/login");

    publicRoutes.insert("/register");

    publicRoutes.insert("/favicon.ico");
}

void AuthMiddleware::allow(
    const string& path)
{
    publicRoutes.insert(path);
}

bool AuthMiddleware::isPublicRoute(
    const string& path) const
{
    return publicRoutes.find(path)
        != publicRoutes.end();
}

bool AuthMiddleware::validateToken(
    const string& token) const
{
    //
    // Temporary implementation.
    // Replace later with JWT/session/API-key verification.
    //

    return token == "Lothal";
}

void AuthMiddleware::handle(
    HttpRequest& request,
    HttpResponse& response,
    Next next)
{
    if(isPublicRoute(request.getPath()))
    {
        next();
        return;
    }

    string auth =
        request.getHeader("Authorization");

    if(auth.empty())
    {
        response.setStatus("401 Unauthorized");

        response.setContentType("text/plain");

        response.setBody(
            "Authorization header required"
        );

        return;
    }

    const string prefix = "Bearer ";

    if(auth.substr(0, prefix.size()) != prefix)
    {
        response.setStatus("401 Unauthorized");

        response.setContentType("text/plain");

        response.setBody(
            "Invalid authorization format"
        );

        return;
    }

    string token =
        auth.substr(prefix.size());

    if(!validateToken(token))
    {
        response.setStatus("403 Forbidden");

        response.setContentType("text/plain");

        response.setBody(
            "Invalid token"
        );

        return;
    }

    next();
}