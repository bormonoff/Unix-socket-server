#pragma once

#include <signal.h>

namespace network {

// Sets SIGPIPE error handler 
void sigpipe_handler(int unused);
}  // namespace network