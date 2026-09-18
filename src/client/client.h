#pragma once
using namespace std;

void connect_client(string ip_target){

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
string send_mes;

while (true)
{
    getline(cin, send_mes);

    send(
        connect_socket,
        send_mes.c_str(),
        send_mes.size(),
        0
    );

    if (send_mes == "EXIT")
        break;

    string recv_mes;
    char buffer[1024];

    while (true)
    {
        int bytes = recv(
            connect_socket,
            buffer,
            sizeof(buffer),
            0
        );

        if (bytes <= 0)
            break;

        recv_mes.append(buffer, bytes);

        // پیدا کردن علامت پایان
        size_t pos = recv_mes.find("<END>");

        if (pos != string::npos)
        {
            recv_mes.erase(pos);
            break;
        }
    }

    cout << recv_mes;
}

}
