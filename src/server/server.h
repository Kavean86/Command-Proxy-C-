#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;


bool send_all(int socket, const char* data, size_t len)
{
    size_t sent = 0;

    while (sent < len)
    {
        ssize_t bytes = send(
            socket,
            data + sent,
            len - sent,
            0
        );

        if (bytes <= 0)
            return false;

        sent += bytes;
    }

    return true;
}


void handle_client(int client)
{
    char buffer[1024];

    while (true)
    {
        int bytes = recv(
            client,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';

        string command(buffer);

        // خروج
        if (command == "EXIT")
            break;


        /*
         * cd باید در خود server اجرا شود
         * تا directory تغییرکرده باقی بماند.
         */
        if (command.rfind("cd ", 0) == 0)
        {
            string path = command.substr(3);

            if (chdir(path.c_str()) != 0)
            {
                const char* error = "cd: cannot change directory\n";

                send_all(
                    client,
                    error,
                    strlen(error)
                );
            }
        }
        else
        {
            /*
             * 2>&1
             *
             * stderr را هم وارد stdout می‌کند
             * تا خطاها هم از pipe خوانده شوند.
             */
            string full_command = command + " 2>&1";

            FILE* pipe = popen(
                full_command.c_str(),
                "r"
            );

            if (!pipe)
            {
                const char* error = "popen: failed\n";

                send_all(
                    client,
                    error,
                    strlen(error)
                );
            }
            else
            {
                while (fgets(
                    buffer,
                    sizeof(buffer),
                    pipe
                ))
                {
                    size_t len = strlen(buffer);

                    if (!send_all(
                        client,
                        buffer,
                        len
                    ))
                    {
                        pclose(pipe);
                        close(client);
                        return;
                    }
                }

                pclose(pipe);
            }
        }


        /*
         * اعلام پایان خروجی command
         */
        const char* END = "<END>";

        if (!send_all(
            client,
            END,
            strlen(END)
        ))
        {
            break;
        }
    }

    close(client);
}


/*
 * ساخت و اجرای کامل server
 */
void start_server(int port)
{
    int server_socket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (server_socket < 0)
    {
        perror("socket");
        return;
    }


    int opt = 1;

    setsockopt(
        server_socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );


    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);


    if (bind(
        server_socket,
        (sockaddr*)&server_addr,
        sizeof(server_addr)
    ) < 0)
    {
        perror("bind");
        close(server_socket);
        return;
    }


    if (listen(server_socket, 5) < 0)
    {
        perror("listen");
        close(server_socket);
        return;
    }


    cout << "Server listening on port "
         << port
         << "...\n";


    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client = accept(
            server_socket,
            (sockaddr*)&client_addr,
            &client_len
        );

        if (client < 0)
        {
            perror("accept");
            continue;
        }


        cout << "Client connected.\n";


        handle_client(client);


        cout << "Client disconnected.\n";
    }


    close(server_socket);
}
