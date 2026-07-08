#include "server.h"
#pragma comment(lib, "Ws2_32.lib")

int main(){
    
    Server server(8080);

    if(!server.initialize()){
        return 1;
    }

    server.get("/api/hello", [](HttpRequest& req, HttpResponse& res){
        res.setStatus("200 OK");
        res.setContentType("text/plain");
        res.setBody("Hello from Lothal!");
    });

    server.get("/users/:id",
[](HttpRequest& req, HttpResponse& res)
{
    res.setBody(
        req.getParam("id")
    );
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

    server.post("/login", [](HttpRequest& req, HttpResponse& res)
{
    res.setStatus("200 OK");
    res.setContentType("text/plain");
    res.setBody("Login Success");
});

    server.start();

    return 0;
}