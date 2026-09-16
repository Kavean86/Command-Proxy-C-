#pragma once
using namespace std;

void user_connect(string ip_target){

int connect_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

if(connect_socket<0){
perror("socket");
return;
}

sockaddr_in addr{};
addr.sin_family=AF_INET;
addr.sin_port=htons(6286);

if(inet_pton(AF_INET,ip_target.c_str(),&addr.sin_addr)<0){
perror("inet_pton");
close(connect_socket);
return;
}
int connecting=connect(connect_socket,(sockaddr*)&addr,sizeof(addr));
if(connecting<0){
perror("connect");
close(connect_socket);
return;
}

if(connecting==0){
cout<<"connect to server <"<<ip_target<<">\n";
}
}
