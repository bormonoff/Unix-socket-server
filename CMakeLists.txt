cmake_minimum_required(VERSION 3.11)

project(UNIXServer CXX)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)
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
# server source ---------