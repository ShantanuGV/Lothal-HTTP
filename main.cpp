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

    server.start();

    return 0;
}