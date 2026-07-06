#include "server.h"
#pragma comment(lib, "Ws2_32.lib")

int main(){
    
    Server server(8080);

    if(!server.initialize()){
        return 1;
    }

    server.start();

    return 0;
}