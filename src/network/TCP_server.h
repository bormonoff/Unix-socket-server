#pragma once

#include <iostream>
#include <thread>
#include <functional>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "handlers/handler.h"
#include "util/util.h"

namespace network {

const unsigned short PORT = 8888;
const unsigned short MAX_CONNECTIONS_COUNT = 15;
const unsigned short CHAR_BUF = 50;

class TCPServer {
public:
    using Socket = int;

    TCPServer() = default;

    TCPServer(const TCPServer&) = delete;
    TCPServer operator=(const TCPServer&) = delete;

    // Inits the server socket and runs Run() function
    void Start();

    // Reads from a socket into buffer. Checks if the connection is available.
    void ReadFromSocket(std::string& buff, Socket client_socket);
    
    // Sends a message to a client
    void SendMessage(const std::string& seq, Socket client_socket);

    // Closes client socket. Often calls on client disconnection
    void CloseClientSocket(Socket client_socket);

    ~TCPServer() { close(server_socket_); }

private:
    // Accepts new clients via the while cycle and creates 
    // threads using handlers to handle the clients
    void Run();

    // Sets handlers to correctly handle error signals
    void SetSignalHandlers();

    Socket server_socket_;
    uint16_t port_;
};
}  // namespace network
