#include <iostream>
#include <thread>
#include <functional>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "util/util.h"

namespace network {

const unsigned short PORT = 8888;
const unsigned short MAX_CONNECTIONS_COUNT = 15;
const unsigned short CHAR_BUF = 50;

struct SequenseInfo {
    std::map<std::string, std::pair<uint16_t, uint16_t>> sequenses;
};

class TCPServer {
public:
    using Socket = int;

    TCPServer() = default;

    TCPServer(const TCPServer&) = delete;
    TCPServer operator=(const TCPServer&) = delete;

    // Init the server socket and run Run() function
    void Start();

    ~TCPServer() { close(server_socket_); }

private:
    // This function accept new clients via while cycle and
    // move the socket to a handler
    void Run();

    // Additional funtion that write error msg in the socket
    void SendError(const std::string& seq, Socket client_socket);

    // Additional funtion that send a message to a client
    void SendMessage(const std::string& msg, Socket client_socket);

    // This function handle the new client and handle it
    void HandleRequest(Socket client_socket);

    // This funtion correctly close client socket after disconnection 
    void CloseClientSocket(Socket client_socket);

    Socket server_socket_;
    uint16_t port_;
};
}  // namespace network