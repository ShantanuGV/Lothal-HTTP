#include "LoggerMiddleware.h"

#include <iostream>

using namespace std;

LoggerMiddleware::LoggerMiddleware()
{
}

void LoggerMiddleware::handle(
    HttpRequest& request,
    HttpResponse& response,
    Next next)
{
    cout << "\n========== REQUEST ==========\n";

    cout << request.getMethod()
         << " "
         << request.getPath()
         << endl;

    cout << "HTTP Version : "
         << request.getVersion()
         << endl;

    cout << "=============================\n";

    next();
}