#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "client.h"

using namespace std;

// Program entry point
int main(int argc,char* argv[]){

    // Check if the server IP address was provided
    if(argc < 2){
        cerr<<"invalid arguments\n";
        return 1;
    }

    // Get the server IP address from command-line arguments
    string ip=argv[1];

    // Display connection message
    cout<<"Connect to server...\n";

    // Connect to the server
    connect_client(ip);
}
