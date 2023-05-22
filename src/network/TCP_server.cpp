#include "network/TCP_server.h"

#include <sstream>

#include "network/signal_handlers.h"

namespace network {
using namespace std::literals;

void TCPServer::Start() {
    // Setup the socket and connection tools: PORT and interfaces
    sockaddr_in servAddr; 
    // This function needs to make all fields zero in a struct
    bzero((char*)&servAddr, sizeof(servAddr));
    // AF_INET - for an IPV4
    servAddr.sin_family = AF_INET; 
    // INADDR_ANY bind server socket to all interfaces not to only localhost
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servAddr.sin_port = htons(8888);

    // Create a socket with a descriptor
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket_ < 0) {
        std::cerr << "Can't create the server socket"s << std::endl;
        exit(0);
    }

    // Bind the socket to the server address struct
    int bindStatus = bind(server_socket_, (struct sockaddr*) &servAddr, 
                          sizeof(servAddr));
    if(bindStatus < 0) {
        std::cerr << "Error binding socket to the local address. "s
                  << "Wait and reboot..."s << std::endl;
        exit(0);
    }

    std::cout << "Waiting for a client to connect..."s << std::endl;

    // Server now listen the socket
    listen(server_socket_, MAX_CONNECTIONS_COUNT);

    SetSignalHandlers();

    // Inits server funtion
    Run();
    
}

void TCPServer::Run() {
    socklen_t addrlen = sizeof(sockaddr_in);
    while (true) {
        sockaddr_in client_addr;
        // Accepts a new client and creates client socket
        Socket client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, 
                                      &addrlen);
        std::cout << "Client has connected. Socket: "s << client_socket << std::endl;

        // Creates an addiotional thread to handle the connetction
        std::thread(handler::HandleClientConnection<Socket, TCPServer>,
                    client_socket, std::ref(*this)).detach();
    }
}

void TCPServer::SetSignalHandlers() {
    signal(SIGPIPE, sigpipe_handler);
}

void TCPServer::ReadFromSocket(std::string& buff, Socket client_socket) {
    auto seq_bytes = recv(client_socket, buff.data(), buff.size() + 1, 0);
    if (seq_bytes <= 0) {
        throw std::runtime_error("Client has disconnected"s);
    }
}

void TCPServer::SendMessage(const std::string& msg, Socket client_socket) {
    auto send_bytes = send(client_socket, msg.c_str(), msg.size() + 1, 0);
    if (send_bytes <= 0) {
        throw std::runtime_error("Client has disconnected"s);
    }
}

void TCPServer::CloseClientSocket(Socket client_socket) {
    std::cout << "Client has disconnected. Socket: "s <<  client_socket << std::endl;
    close(client_socket);
}
}  // namespace network
