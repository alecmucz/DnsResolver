cmake_minimum_required(VERSION 3.30) # C++26 Support
project(DnsResolver)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SOCKET LIBRARY
add_library(socket STATIC
        src/socket/IpAddress.cpp
        src/socket/SocketAddress.cpp
        include/socket/DatagramSocket.h
        src/socket/DatagramSocket.cpp)
target_include_directories(socket
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(socket
        PRIVATE
        -Wall -Wextra -Wpedantic)

# DNS LIBRARY
add_library(dns STATIC
        src/dns/Message.cpp
        src/dns/DomainName.cpp)
target_include_directories(dns
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(dns
        PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion)


# CMAKE DEPENDENCY
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.3)
FetchContent_MakeAvailable(Catch2)
enable_testing()

# MAIN EXECUTABLE (APP)
add_executable(DnsResolver main.cpp)
target_link_libraries(DnsResolver
        PRIVATE
        socket dns)
target_compile_options(
        DnsResolver
        PRIVATE
        -Wall -Wextra -Wpedantic)
