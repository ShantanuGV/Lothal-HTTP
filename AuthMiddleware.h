#pragma once

#include <unordered_set>
#include <string>

#include "Middleware.h"

class AuthMiddleware : public Middleware
{
public:

    AuthMiddleware();

    void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) override;

    void allow(const std::string& path);

private:

    std::unordered_set<std::string> publicRoutes;

    bool isPublicRoute(
        const std::string& path) const;

    bool validateToken(
        const std::string& token) const;
};