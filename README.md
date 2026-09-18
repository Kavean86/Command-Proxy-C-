# TCP Remote Command Server

A lightweight **client-server remote command execution application** written in **C++** for Linux.

The project uses **TCP sockets** to establish communication between a client and server. The server can handle multiple clients simultaneously using C++ threads and execute commands received from connected clients.

This project was built to explore **low-level network programming, socket communication, multithreading, process management, and Linux system programming**.

## Features

* 🔌 TCP socket communication
* 🖥️ Client-server architecture
* 👥 Multiple clients supported simultaneously
* 🧵 One thread per connected client
* 💻 Remote command execution
* 📂 Remote `cd` command support
* 📡 Reliable data transmission with `send_all()`
* 🔄 Response termination using `<END>` marker
* 🐧 Designed for Linux
* ⚙️ Uses Linux system calls and process management functions

## Project Structure

```text
.
├── Client/
│   ├── main.cpp
│   └── client.h
│
└── Server/
    ├── main.cpp
    └── server.h
```

## How It Works

The server listens for incoming TCP connections on port `6286`.

When a client connects:

1. The client establishes a TCP connection with the server.
2. The server accepts the connection.
3. A new thread is created for the client.
4. The client sends a command.
5. The server executes the command.
6. The command output is sent back to the client.
7. The server sends `<END>` to indicate that the response is complete.
8. The client displays the response.

Multiple clients can be connected at the same time because each client is handled by a separate thread.

```text
             ┌──────────────┐
             │    Server    │
             │   TCP :6286  │
             └──────┬───────┘
                    │
          ┌─────────┼─────────┐
          │         │         │
       Thread 1  Thread 2  Thread 3
          │         │         │
       Client 1  Client 2  Client 3
```

## Requirements

* Linux
* G++
* C++11 or newer
* POSIX sockets
* pthread support

## Compilation

### Server

```bash
cd Server
g++ main.cpp -o server -pthread
```

### Client

```bash
cd Client
g++ main.cpp -o client -pthread
```

## Usage

### Start the Server

```bash
./server
```

The server listens on:

```text
0.0.0.0:6286
```

### Start the Client

```bash
./client <SERVER_IP>
```

Example:

```bash
./client 192.168.1.100
```

After connecting, commands can be entered directly:

```text
ls
pwd
whoami
uname -a
```

To close the client connection:

```text
EXIT
```

## `cd` Support

The client can also send:

```text
cd /tmp
```

The server attempts to change its current working directory and subsequent commands are executed from that directory.

## Example

Client:

```text
Connect to server...
connect to server <192.168.1.100>

$ ls
client
server

$ pwd
/home/user/project

$ whoami
user
```

## Technologies

* **C++**
* **TCP/IP**
* **POSIX Sockets**
* **Multithreading**
* **Linux System Calls**
* `socket()`
* `bind()`
* `listen()`
* `accept()`
* `connect()`
* `send()`
* `recv()`
* `fork()`-style process concepts through command execution
* `popen()`
* `pclose()`
* `chdir()`

## Learning Goals

This project focuses on understanding:

* TCP client-server communication
* Socket lifecycle
* Blocking I/O
* Handling partial `send()` operations
* Receiving variable-length data
* Multithreaded server architecture
* Linux process and command execution
* Communication protocols and message boundaries

## Security Notice

This project is intended for **learning and authorized lab environments**.

The server executes commands received from connected clients, so exposing it to an untrusted network can allow unauthorized command execution on the host.

Do not run it on systems or networks where you do not have explicit authorization.

## Future Improvements

Possible improvements include:

* Client authentication
* Encrypted communication using TLS
* Better message framing
* Per-client working directories
* Graceful thread management
* Connection logging
* Command restrictions
* Improved error handling
* Authentication and authorization
* IPv6 support
* Better handling of concurrent clients
