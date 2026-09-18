#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "server.h"

using namespace std;

// Program entry point
int main(){

    // Start the server on port 6286
    start_server(6286);
}
