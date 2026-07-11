#pragma once

#include <string>

#include "Middleware.h"

class StaticFileMiddleware : public Middleware
{
public:

    explicit StaticFileMiddleware(
        const std::string& root = "public");

    void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) override;

private:

    std::string rootDirectory;

    bool isStaticRequest(
        const std::string& path) const;

    std::string buildPath(
        const std::string& path) const;

    bool isSafePath(
        const std::string& path) const;
};