#include "network/TCP_server.h"

#include <cmath>

namespace {

void InitServer() {
    network::TCPServer server{};
    server.Start();
}
}  // namespace 

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {  
    InitServer();
    return 0;   
}