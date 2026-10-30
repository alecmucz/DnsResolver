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
        src/dns/DomainName.cpp
        src/dns/Wire.cpp
        src/dns/Wire.h
)
target_include_directories(dns
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(dns
        PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion)

# RESOLVER LIBRARY
add_library(resolver STATIC
        src/resolver/Resolver.cpp
        include/resolver/Resolver.h)
target_include_directories(resolver
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(resolver
        PUBLIC
        dns socket)
target_compile_options(resolver
        PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion)

# TEST (CATCH2) DEPENDENCY
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.3)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
enable_testing()
include(Catch)
add_executable(DnsResolverTest
        tests/DomainName_test.cpp
        tests/Message_test.cpp
)

target_include_directories( DnsResolverTest
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(DnsResolverTest
        PRIVATE
        dns Catch2::Catch2WithMain)
target_compile_options(DnsResolverTest
        PRIVATE
        -Wall -Wextra -Wpedantic
)
catch_discover_tests(DnsResolverTest)

# MAIN EXECUTABLE (APP)
add_executable(DnsResolver main.cpp)
set_target_properties(DnsResolver PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(DnsResolver
        PRIVATE
        resolver)
target_compile_options(
        DnsResolver
        PRIVATE
        -Wall -Wextra -Wpedantic)