#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class Server{
    private:
        int port;
        WSADATA wsaData;
        SOCKET serverSocket;
        SOCKET clientSocket;
        sockaddr_in serverAddress;

    public:
        Server(int port);
        ~Server();
        bool initialize();
        void start();
        void shutdown();
};