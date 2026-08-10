#include <iostream>
#include <utility>
#include "include/socket/FileDescriptor.h"

int main() {
    FileDescriptor fd_a{4};
    FileDescriptor fd_b{5};

    std::print("Before move:\n");
    std::print("fd_a = {}\n", fd_a.value());
    std::print("fd_b = {}\n", fd_b.value());

    fd_a = std::move(fd_b);

    std::print("After move:\n");
    std::print("fd_a = {}\n", fd_a.value());
    std::print("fd_b = {}\n", fd_b.value());
    return 0;
}