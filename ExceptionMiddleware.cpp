#include "ExceptionMiddleware.h"

#include <exception>
#include <iostream>

using namespace std;

ExceptionMiddleware::ExceptionMiddleware()
{
}

void ExceptionMiddleware::handle(
    HttpRequest& request,
    HttpResponse& response,
    Next next)
{
    try
    {
        next();
    }
    catch(const exception& e)
    {
        cerr
            << "\n===== Unhandled Exception =====\n"
            << e.what()
            << endl;

        response.setStatus("500 Internal Server Error");
        response.setContentType("text/plain");
        response.setBody(
            "500 Internal Server Error"
        );
    }
    catch(...)
    {
        cerr
            << "\n===== Unknown Exception =====\n";

        response.setStatus("500 Internal Server Error");
        response.setContentType("text/plain");
        response.setBody(
            "500 Internal Server Error"
        );
    }
}