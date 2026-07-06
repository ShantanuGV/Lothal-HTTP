#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main(){
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (result != 0){
        cout<<"WSAStartup failed : "<<result<<"\n";
        return 1;
    }
    cout<<"Winsocket initialized!!!!!!!\n";

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET){
        cout<<"Socket creation failed.\n";

        WSACleanup();

        return 1;
    }
    cout<<"Socket created successfully!!!\n";

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR){

        cout<<"Bind Failed!\n";

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }
    cout<<"Bind Created Succesfully!!!!\n";
    
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR){
        cout<<"Listen failed!\n";

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }
    cout<<"Server is now listening on port 8080...\n";

    sockaddr_in clientAddress;
    int clientSize = sizeof(clientAddress);

    SOCKET clientSocket = accept(serverSocket,(sockaddr*)&clientAddress,&clientSize);

    char buffer[4096];

    if (clientSocket == INVALID_SOCKET){
        cout<<"Accept faild!\n";

        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }
    cout<<"Client Connected!!!!!!!!!\n";

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived == SOCKET_ERROR){
        cout<<"Receive failed!\n";

        closesocket(serverSocket);
        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }

    buffer[bytesReceived] = '\0';

    cout << "\n========== HTTP REQUEST ==========\n";
    cout << buffer;
    cout << "==================================\n";


    const char* body = "Hello, World!!!";
    string response = "HTTP/1.1 200 OK\r\n""Content-Type: text/plain\r\n""Content-Length:" + to_string(strlen(body)) + "\r\n""\r\n" + body;

    int bytesSent = send(clientSocket, response.c_str(), response.size(), 0);

    cout<<"Response sent!!!!!\n";

    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Send failed: " << WSAGetLastError() << '\n';
    }
    else
    {
        cout << "Sent " << bytesSent << " bytes\n";
    }


    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();


    return 0;
}