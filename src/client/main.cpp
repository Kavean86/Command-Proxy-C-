#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "client.h"

using namespace std;

int main(int argc,char* argv[]){

if(argc < 2){
cerr<<"invalid arguments\n";
return 1;
}
string ip=argv[1];

cout<<"Connect to server...\n";
connect_client(ip);
}
