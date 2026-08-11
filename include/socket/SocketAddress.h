#pragma once

#include <cstdint>
#include "IpAddress.h"

class SocketAddress {
private:
    IpAddress address;
    std::uint16_t port{0};

public:
    SocketAddress() = delete;
    explicit SocketAddress(
        const IpAddress &address,
        const std::uint16_t port
        ) : address(address), port(port) {}

};
