#include <iostream>

#include "RateLimitMiddleware.h"


using namespace std;


RateLimitMiddleware::RateLimitMiddleware(
    int maxRequests,
    int windowSeconds)
:
maxRequests(maxRequests),
windowSeconds(windowSeconds)
{

}



string RateLimitMiddleware::getClientIP(
    HttpRequest& request)
{

    /*
        Later this can support:

        X-Forwarded-For
        Proxy headers
        Real socket address

        For now:
        using client IP header
    */


    return request.getClientIP();

}





bool RateLimitMiddleware::isAllowed(
    const string& ip)
{

    auto now =
        chrono::steady_clock::now();



    auto it =
        clients.find(ip);



    // First request
    if(it == clients.end())
    {
        clients[ip] =
        {
            1,
            now
        };

        return true;
    }



    ClientInfo& client =
        it->second;



    auto elapsed =
        chrono::duration_cast<
            chrono::seconds
        >
        (
            now - client.windowStart
        )
        .count();



    // Reset window

    if(elapsed >= windowSeconds)
    {
        client.requests = 1;

        client.windowStart = now;

        return true;
    }



    // Limit exceeded

    if(client.requests >= maxRequests)
    {
        return false;
    }



    client.requests++;

    return true;
}





void RateLimitMiddleware::handle(
    HttpRequest& request,
    HttpResponse& response,
    Next next)
{

    string ip =
        getClientIP(request);

    cout << "Client IP: " << ip << endl;    



    bool allowed;


    {
        lock_guard<mutex> lock(rateMutex);

        allowed =
            isAllowed(ip);
    }



    if(!allowed)
    {

        response.setStatus(
            "429 Too Many Requests"
        );


        response.setContentType(
            "text/plain"
        );


        response.setBody(
            "Rate limit exceeded"
        );

        


        return;
    }



    next();
}