#include "network/TCP_server.h"

namespace network {

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

    // Create socket with a descriptor
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket_ < 0) {
        std::cerr << "Can't create the server socket" << std::endl;
        exit(0);
    }

    // Bind the socket to the server address struct
    int bindStatus = bind(server_socket_, (struct sockaddr*) &servAddr, 
                        sizeof(servAddr));
    if(bindStatus < 0) {
        std::cerr << "Error binding socket to local address. Wait and reboot..." << std::endl;
        exit(0);
    }

    std::cout << "Waiting for a client to connect..." << std::endl;

    // Server now listen the socket
    listen(server_socket_, MAX_CONNECTIONS_COUNT);

    // Init server funtion
    Run();
}

void TCPServer::HandleRequest(int client_socket) {
    char msg[500];
    while(true) {
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        auto seq1_bytes = recv(client_socket, (char*)&msg, sizeof(msg), 0);
        if (seq1_bytes == 0) {
            std::cout << "client has broken the connetction" << std::endl;
            break;
        }
        auto seq1_data = util::ParseStringToSeq("seq1", msg);
        if (!seq1_data.has_value()) {
            SendError(client_socket);
        }
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        auto seq2_bytes = recv(client_socket, (char*)&msg, sizeof(msg), 0);
        if (seq2_bytes == 0) {
            std::cout << "client has broken the connetction" << std::endl;
            break;
        }
        auto seq2_data = util::ParseStringToSeq("seq1", msg);
        if (!seq2_data.has_value()) {
            SendError(client_socket);
        }
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        auto seq3_bytes = recv(client_socket, (char*)&msg, sizeof(msg), 0);
        if (seq3_bytes == 0) {
            std::cout << "client has broken the connetction" << std::endl;
            break;
        }
        auto seq3_data = util::ParseStringToSeq("seq1", msg);
        if (!seq3_data.has_value()) {
            SendError(client_socket);
        }
    }
    close(client_socket);
}

void TCPServer::Run() {
    socklen_t addrlen = sizeof(sockaddr_in);
    while (true) {
        sockaddr_in client_addr;
        // Connect a new client
        Socket client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &addrlen);

        // Create an addiotional thread to handle the connetction
        std::thread(&TCPServer::HandleRequest, this, client_socket).detach();
    }
}

void TCPServer::SendError(int client_socket) {
    char msg[30] = "Invalid format try again";
    send(client_socket, (char*)&msg, strlen(msg), 0);
}
}  // namespace network
