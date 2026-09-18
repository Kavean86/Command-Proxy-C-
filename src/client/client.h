#pragma once

using namespace std;

// Connect to the target server
void connect_client(string ip_target){

    // Create a TCP socket
    int connect_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    // Check if socket creation failed
    if(connect_socket<0){
        perror("socket");
        return;
    }

    // Create and initialize the server address structure
    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(6286);

    // Convert the target IP address from string to binary format
    if(inet_pton(AF_INET,ip_target.c_str(),&addr.sin_addr)<0){
        perror("inet_pton");
        close(connect_socket);
        return;
    }

    // Connect to the server
    int connecting=connect(connect_socket,(sockaddr*)&addr,sizeof(addr));

    // Check if the connection failed
    if(connecting<0){
        perror("connect");
        close(connect_socket);
        return;
    }

    // Connection was successful
    if(connecting==0){
        cout<<"connect to server <"<<ip_target<<">\n";
    }

    // Store the command entered by the user
    string send_mes;

    while (true)
    {
        // Set terminal text color to yellow
        cout << "\033[33m";

        // Read the user's command
        getline(cin, send_mes);

        // Reset terminal text color
        cout << "\033[0m";

        // Send the command to the server
        send(
            connect_socket,
            send_mes.c_str(),
            send_mes.size(),
            0
        );

        // Exit the client if the user sends EXIT
        if (send_mes == "EXIT")
            break;

        // Store the response received from the server
        string recv_mes;
        char buffer[1024];

        while (true)
        {
            // Receive data from the server
            int bytes = recv(
                connect_socket,
                buffer,
                sizeof(buffer),
                0
            );

            // Stop receiving if the connection is closed or an error occurs
            if (bytes <= 0)
                break;

            // Append the received data to the response string
            recv_mes.append(buffer, bytes);

            // Search for the end marker sent by the server
            size_t pos = recv_mes.find("<END>");

            // Check if the complete response has been received
            if (pos != string::npos)
            {
                // Remove the end marker from the response
                recv_mes.erase(pos);

                // Stop receiving the current response
                break;
            }
        }

        // Display the server response
        cout << recv_mes;
    }

}
