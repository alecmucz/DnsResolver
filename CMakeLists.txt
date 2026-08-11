cmake_minimum_required(VERSION 4.3)
project(DnsResolver)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SOCKET LIBRARY
add_library(socket STATIC
        src/socket/IpAddress.cpp)
target_include_directories(socket
                    PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/include)


# CMAKE DEPENDENCY
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.3
)
FetchContent_MakeAvailable(Catch2)
enable_testing()

# MAIN EXECUTABLE (APP)
add_executable(DnsResolver main.cpp)
target_link_libraries(DnsResolver
                    PRIVATE
                    socket)
