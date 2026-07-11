#pragma once

#include <unordered_map>
#include <string>
#include <chrono>
#include <mutex>

#include "Middleware.h"


class RateLimitMiddleware : public Middleware
{
public:

    RateLimitMiddleware(
        int maxRequests = 100,
        int windowSeconds = 60
    );


    void handle(
        HttpRequest& request,
        HttpResponse& response,
        Next next
    ) override;


private:

    struct ClientInfo
    {
        int requests;

        std::chrono::steady_clock::time_point windowStart;
    };


    std::unordered_map<
        std::string,
        ClientInfo
    > clients;


    int maxRequests;

    int windowSeconds;


    std::mutex rateMutex;


    std::string getClientIP(
        HttpRequest& request
    );


    bool isAllowed(
        const std::string& ip
    );
};