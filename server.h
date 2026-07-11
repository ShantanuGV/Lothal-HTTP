#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#include "router.h"
#include "MiddlewarePipeline.h"

class Server{
    private:
        int port;
        WSADATA wsaData;
        SOCKET serverSocket;
        SOCKET clientSocket;
        sockaddr_in serverAddress;
        Router router;
        MiddlewarePipeline pipeline;

    public:
        Server(int port);
        ~Server();
        bool initialize();
        void start();
        void shutdown();

        void get(const string& path, Router::Handler handler);

        void post(const string& path, Router::Handler handler);

        void put(const string& path, Router::Handler handler);

        void patch(const string& path, Router::Handler handler);

        void del(const string& path,
            Router::Handler handler);

        void head(const string& path, Router::Handler handler);

        void options(const string& path, Router::Handler handler);

        void query(const string& path, Router::Handler handler);

        void use(std::shared_ptr<Middleware> middleware);

        void use(
    const std::string& prefix,
    std::shared_ptr<Middleware> middleware
);
};