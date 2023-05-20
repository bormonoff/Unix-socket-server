#include "network/TCP_server.h"

#include <sstream>

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
        std::cerr << "Error binding socket to the local address. Wait and reboot..." << std::endl;
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
    SequenseInfo client_data;
    try {
        std::vector<std::string> seq_name{"seq1", "seq2", "seq3"};
        bool cycle {true};
        while(cycle) {
            for (auto& seq : seq_name) {
                memset(&msg, 0, sizeof(msg)); 
                auto seq1_bytes = recv(client_socket, (char*)&msg, sizeof(msg), 0);
                if (seq1_bytes <= 0) {
                    throw std::runtime_error("Client has disconnected");
                }
                auto seq_data = util::ParseStringToSeq(seq, msg);
                if (!seq_data.has_value()) {
                    SendError(seq, client_socket);
                    break;
                }
                client_data.sequenses[seq] = seq_data.value();
                if (seq == "seq3") { cycle = false; }
                std::string result = seq + " was successfully added. Start: " + std::to_string(seq_data.value().first)
                                      + ". Interval: " + std::to_string(seq_data.value().second) + "\r\n";
                SendMessage(result, client_socket);
            }
        }
    } catch (std::exception& ex) {
        CloseClientSocket(client_socket);
        return;
    }


    int64_t first_start = client_data.sequenses.find("seq1") -> second.first;
    int64_t second_start = client_data.sequenses.find("seq2") -> second.first;
    int64_t third_start = client_data.sequenses.find("seq3") -> second.first;

    int16_t first_interval = client_data.sequenses.find("seq1") -> second.second;
    int16_t second_interval = client_data.sequenses.find("seq2") -> second.second;
    int16_t third_interval = client_data.sequenses.find("seq3") -> second.second;
    while (true) {
        // Buildid_add_overflow checks is int overflow is happened
        // Change the variable. https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
        if (__builtin_add_overflow(first_start, first_interval, &first_start)) {
            first_start = first_interval;
        }
        if (__builtin_add_overflow(second_start, second_interval, &second_start)) {
            second_start = second_interval = 1000;
        }
        if (__builtin_add_overflow(third_start, third_interval, &third_start)) {
            third_start = third_interval = 1000;
        }

        std::string result = "1: " + std::to_string(first_start) + "  2:  " + std::to_string(second_start) 
                              +  "  3:  " + std::to_string(third_start) + "\r\n";

        auto bytes = send(client_socket, result.c_str(), result.size() + 1, 0);    
        if (bytes <= 0) {
            CloseClientSocket(client_socket);
            return;
        }
    }
}

void TCPServer::CloseClientSocket(Socket client_socket) {
    std::cout << "Client has disconnected. Socket: " <<  client_socket << std::endl;
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

void TCPServer::SendError(const std::string& seq, int client_socket) {
    using namespace std::literals;

    std::string result = "Invalid "s + seq + " format. Please try again from the beginning...\r\n"s;
    auto send_bytes = send(client_socket, result.c_str(), result.size() + 1, 0);
    if (send_bytes <= 0) {
        throw std::runtime_error("Client has disconnected");
    }
}

void TCPServer::SendMessage(const std::string& msg, int client_socket) {
    using namespace std::literals;

    auto send_bytes = send(client_socket, msg.c_str(), msg.size() + 1, 0);
    if (send_bytes <= 0) {
        throw std::runtime_error("Client has disconnected");
    }
}
}  // namespace network
