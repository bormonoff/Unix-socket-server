cmake_minimum_required(VERSION 3.11)

project(UNIXServer CXX)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# server source ---------
add_executable(server 
    src/main.cpp
    src/network/TCP_server.h
    src/network/TCP_server.cpp
    src/network/signal_handlers.h
    src/network/signal_handlers.cpp
    src/handlers/handler.h
    src/util/util.h
    src/util/util.cpp
    )

target_include_directories(server PRIVATE src)

target_link_libraries(server
    ${CMAKE_THREAD_LIBS_INIT}
)
# server source ---------