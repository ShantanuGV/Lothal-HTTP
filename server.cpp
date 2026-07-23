#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <thread>

#include "server.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "FileSystem.h"
#include "MimeTypes.h"
#include "router.h"
#include "ThreadPool.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

void Server::use(
    std::shared_ptr<Middleware> middleware)
{
    pipeline.use(middleware);
}

void Server::use(
    const string& prefix,
    shared_ptr<Middleware> middleware)
{
    pipeline.use(prefix, middleware);
}

Server::Server(int port) : port(port), threadPool(4){
    this->port = port;

    serverSocket = INVALID_SOCKET;

    memset(&serverAddress, 0, sizeof(serverAddress));
}

Server::~Server(){
    cout << "\nCleaning up server...\n";

    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
    }
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
    }

    WSACleanup();
}

bool Server::initialize(){
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed!\n";
        return false;
    }

    cout << "Winsock initialized!\n";

    // Create TCP Socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed!\n";
        WSACleanup();
        return false;
    }

    cout << "Socket created successfully!\n";

    // Configure Address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(serverSocket,
             (sockaddr*)&serverAddress,
             sizeof(serverAddress)) == SOCKET_ERROR)
    {
        cout << "Bind failed!\n";

        closesocket(serverSocket);
        WSACleanup();

        return false;
    }

    cout << "Bind successful!\n";

    // Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "Listen failed!\n";

        closesocket(serverSocket);
        WSACleanup();

        return false;
    }

    cout << "Listening on port " << port << "...\n";

    return true;
}

string loadFile(const string& path){

    std::ifstream file(path);

    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

void Server::get(
    const string& path,
    Router::Handler handler)
{
    router.get(path, handler);
}

void Server::post(
    const string& path,
    Router::Handler handler)
{
    router.post(path, handler);
}

void Server::put(
    const string& path,
    Router::Handler handler)
{
    router.put(path, handler);
}

void Server::patch(
    const string& path,
    Router::Handler handler)
{
    router.patch(path, handler);
}

void Server::del(
    const string& path,
    Router::Handler handler)
{
    router.del(path, handler);
}

void Server::head(
    const string& path,
    Router::Handler handler)
{
    router.head(path, handler);
}

void Server::options(
    const string& path,
    Router::Handler handler)
{
    router.options(path, handler);
}

void Server::query(
    const string& path,
    Router::Handler handler)
{
    router.query(path, handler);
}

void Server::handelClient(
    SOCKET clientSocket,
    string clientIP
)
{
    // Paste EVERYTHING from

    cout << "Worker Thread: "
         << std::this_thread::get_id()
         << endl;

    char buffer[4096];

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "Receive failed!\n";

            closesocket(clientSocket);
            return;
        }

        buffer[bytesReceived] = '\0';

        cout << "\n========== HTTP REQUEST ==========\n";
        cout << buffer;
        cout << "\n==================================\n";

        HttpRequest request;

        if (!request.parse(buffer)){
            cout << "Invalid HTTP Request\n";

            closesocket(clientSocket);

            return;
        }

        request.setClientIP(clientIP);

        cout << "Method : "
             << request.getMethod()
             << endl;

        cout << "Path : "
             << request.getPath()
             << endl;

        cout << "Version : "
             << request.getVersion()
             << endl;

        HttpResponse response;

        pipeline.execute(
    request,
    response,
    [&]()
    {
        router.handle(request, response);
    }
);

        string rawResponse = response.build();

        int bytesSent = send(
            clientSocket,
            rawResponse.c_str(),
            rawResponse.size(),
            0
        );

        if (bytesSent == SOCKET_ERROR)
        {
            cout << "Send failed: "
                 << WSAGetLastError()
                 << endl;
        }
        else
        {
            cout << "Response sent ("
                 << bytesSent
                 << " bytes)\n";
        }

        closesocket(clientSocket);

        cout << "Client disconnected.\n";
}

void Server::start(){
    cout << "\n========== SERVER STARTED ==========\n";

    bool running = true;

    while (running){

        sockaddr_in clientAddress;
        int clientSize = sizeof(clientAddress);

        SOCKET clientSocket = accept(
            serverSocket,
            (sockaddr*)&clientAddress,
            &clientSize);

        char* ip = inet_ntoa(clientAddress.sin_addr);

        string clientIP = ip;    

        if (clientSocket == INVALID_SOCKET)
        {
            cout << "Accept failed!\n";
            continue;
        }

        cout << "\nClient Connected!\n";


        threadPool.submit(
            [this, clientSocket, clientIP](){
                handelClient(
                    clientSocket,
                    clientIP
                );
            }
        );        
        
    }
}

