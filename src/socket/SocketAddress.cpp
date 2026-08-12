#include "socket/SocketAddress.h"

const IpAddress& SocketAddress::ip_address() const noexcept {
    return address;
}

std::uint16_t SocketAddress::port() const noexcept {
    return port_;
}

