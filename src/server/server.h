#pragma once
using namespace std;
void connect_server(){

int recv_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if (recv_socket < 0)
    {
        perror("socket");
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6286);
    addr.sin_addr.s_addr = INADDR_ANY;


    if (bind(recv_socket, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");

        close(recv_socket);

        return;
    }


    if (listen(recv_socket, 1) < 0)
    {
        perror("listen");

        close(recv_socket);

        return;
    }


    sockaddr_in clientaddr{};
    socklen_t client_size =sizeof(clientaddr);

    int client = accept(recv_socket, (sockaddr*)&clientaddr, &client_size);

    if (client < 0)
    {
        perror("accept");

        close(recv_socket);

        return;
    }

	char buffer[1024]={0};
	while(true){
	recv(client,buffer,sizeof(buffer),0);
	cout<<buffer<<endl;
	}

    }
