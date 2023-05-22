#include "signal_handlers.h"

#include <iostream>

namespace network {

void sigpipe_handler(int unused) {
    std::cout << "Error: data has not heen successfully delivered" << std::endl;
}
}  // namespace network