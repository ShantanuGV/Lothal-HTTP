#include "server.h"

#include "ExceptionMiddleware.h"
#include "LoggerMiddleware.h"
#include "CorsMiddleware.h"
#include "RateLimitMiddleware.h"
#include "AuthMiddleware.h"
#include "StaticFileMiddleware.h"

#include <memory>
#include <stdexcept>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    Server server(8080);

    if(!server.initialize())
    {
        return 1;
    }

    //
    // Middleware Pipeline
    //

    server.use(make_shared<ExceptionMiddleware>());
    server.use(make_shared<StaticFileMiddleware>("public"));


server.use(make_shared<LoggerMiddleware>());
server.use(make_shared<CorsMiddleware>());

server.use(
    "/api",
    make_shared<AuthMiddleware>());

server.use(
    "/api",
    make_shared<RateLimitMiddleware>(500, 10));

    //
    // Routes
    //

    server.get("/api/hello",
    [](HttpRequest& req, HttpResponse& res)
    {
        res.setStatus("200 OK");
        res.setContentType("text/plain");
        res.setBody("Hello from Lothal!");
    });

    server.get("/users/:id",
    [](HttpRequest& req, HttpResponse& res)
    {
        res.setStatus("200 OK");
        res.setContentType("text/plain");
        res.setBody(
            "User ID = " +
            req.getParam("id")
        );
    });

    server.get("/slow",
[](HttpRequest& req, HttpResponse& res)
{
    std::this_thread::sleep_for(
        std::chrono::seconds(5)
    );

    res.setStatus("200 OK");
    res.setContentType("text/plain");
    res.setBody("Finished");
});

    server.post("/login",
    [](HttpRequest& req, HttpResponse& res)
    {
        res.setStatus("200 OK");
        res.setContentType("text/plain");
        res.setBody("Login Success");
    });

    //
    // Exception test route
    //

    server.get("/crash",
    [](HttpRequest&, HttpResponse&)
    {
        throw std::runtime_error(
            "Intentional crash for testing"
        );
    });

    server.start();

    return 0;
}