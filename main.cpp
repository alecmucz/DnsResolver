#include <iostream>
#include <utility>
#include "include/socket/FileDescriptor.h"
#include "include/socket/IpAddress.h"

int main() {
    std::print("File Descriptor Test Site\n");
    FileDescriptor fd_a{4};
    FileDescriptor fd_b{5};

    std::print("Before move:\n");
    std::print("fd_a = {}\n", fd_a.value());
    std::print("fd_b = {}\n", fd_b.value());

    fd_a = std::move(fd_b);

    std::print("After move:\n");
    std::print("fd_a = {}\n", fd_a.value());
    std::print("fd_b = {}\n", fd_b.value());



    std::print("\nIpAddress Test Site\n");

    auto ipv4_1_Address = IpAddress::from_string("192.168.1.1");
    if (ipv4_1_Address) {
        if (ipv4_1_Address->family() == AF_INET) {
            std::print("Test 1 Address family = IPv4\n");
        } else if (ipv4_1_Address->family() == AF_INET6) {
            std::print("Test 1 Address family = IPv6\n");
        }
    }
    std::print("Test 1 ToString Test = {}\n", ipv4_1_Address->to_string());

    auto ipv6_1_Address = IpAddress::from_string("da01:e598:94b4:5e94:d848:650b:b060:ebdf");
    if (ipv6_1_Address) {
        if (ipv6_1_Address->family() == AF_INET) {
            std::print("Test 1 Address family = IPv4\n");
        } else if (ipv6_1_Address->family() == AF_INET6) {
            std::print("Test 1 Address family = IPv6\n");
        }
    }
    std::print("Test 1 ToString Test = {}\n", ipv6_1_Address->to_string());

    auto ipv6_2_Address = IpAddress::from_string("::1");
    if (ipv6_2_Address) {
        if (ipv6_2_Address->family() == AF_INET) {
            std::print("Test 2 Address family = IPv4\n");
        } else if (ipv6_2_Address->family() == AF_INET6) {
            std::print("Test 2 Address family = IPv6\n");
        }
    }
    std::print("Test 2 ToString Test = {}\n", ipv6_2_Address->to_string());


    auto garbage = IpAddress::from_string("34234.134.3145.13426");
    if (garbage) {
        std::print("Test 3 Address family = IPv4\n");
    } else {
        std::print("Test 3 Address family = IPv6\n");
    }
    std::print("Test 3 ToString Test = {}\n", garbage->to_string());

    return 0;
}