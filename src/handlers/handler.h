#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <chrono>

#include "util/util.h"

namespace handler {

using namespace std::literals;

const uint8_t BUFFER_SIZE = 200;
// Change up to 10ms to debug response
const std::chrono::milliseconds THREAD_WAIT = 0ms;

struct SequenseInfo {
    std::map<std::string, std::pair<uint16_t, uint16_t>> sequenses;
};

// Forms result sequense for a client using template server object
template<typename Socket = int, typename Server>
void SendResponse(Socket& socket, Server& server, SequenseInfo& info) {
    int64_t first_start = info.sequenses.find("seq1") -> second.first;
    int64_t second_start = info.sequenses.find("seq2") -> second.first;
    int64_t third_start = info.sequenses.find("seq3") -> second.first;

    int16_t first_interval = info.sequenses.find("seq1") -> second.second;
    int16_t second_interval = info.sequenses.find("seq2") -> second.second;
    int16_t third_interval = info.sequenses.find("seq3") -> second.second;

    while (true) {
        std::string result =  "1: " + std::to_string(first_start) + 
                            "  2:  " + std::to_string(second_start) +
                            "  3:  " + std::to_string(third_start) + "\r\n";

        server.SendMessage(result, socket); 

        // Buildid_add_overflow checks if int overflow happens. Changes 
        // the variable. https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
        if (__builtin_add_overflow(first_start, first_interval, &first_start)) {
            first_start = info.sequenses.find("seq1") -> second.first;
        }
        if (__builtin_add_overflow(second_start, second_interval, &second_start)) {
            second_start = info.sequenses.find("seq2") -> second.first;
        }
        if (__builtin_add_overflow(third_start, third_interval, &third_start)) {
            third_start = info.sequenses.find("seq3") -> second.first;
        }
        std::this_thread::sleep_for(THREAD_WAIT);
    }
}

// Gets a sequence data from a client and form SequenseInfo struct usinig template
// server object
template<typename Socket = int, typename Server>
void FormSeqData(Socket& socket, Server& server, SequenseInfo& info) {
    std::string buff;    
    buff.resize(BUFFER_SIZE);
    bool cycle {true};

    std::vector<std::string> seq_name{"seq1", "seq2", "seq3"};
    while(cycle) {
        for (auto& seq : seq_name) {
            server.ReadFromSocket(buff, socket);
            auto seq_data = util::ParseStringToSeq(seq, buff);
            if (!seq_data.has_value()) {
                std::string str_error = "Invalid "s + seq + 
                                        " format. Please try again from the beginning...\r\n"s;
                server.SendMessage(str_error, socket);
                break;
            }
            info.sequenses[seq] = seq_data.value();
            if (seq == "seq3") { cycle = false; }
            std::string result = seq + " was successfully added. Start: "s + 
                                 std::to_string(seq_data.value().first) + ". Interval: "s + 
                                 std::to_string(seq_data.value().second) + "\r\n";
            server.SendMessage(result, socket);
        }
    }   

    server.SendMessage("Write \"export seq\" to get a sequence\r\n", socket);
    while(true) {
        server.ReadFromSocket(buff, socket);
        if (buff.find("export seq") != std::string::npos) {
            break;
        }
        server.SendMessage("Error, write \"export seq\" again\r\n", socket);
    }
}

// Funtion handle client requests using template server and socket
template<typename Socket = int, typename Server>
void HandleClientConnection(Socket&& socket, Server& server) {
    SequenseInfo client_data;
    try {
        FormSeqData(socket, server, client_data);
        SendResponse(socket, server, client_data);
    } catch (std::exception& ex) {
        server.CloseClientSocket(socket);
        return;
    }
}
}  // namespace handler
