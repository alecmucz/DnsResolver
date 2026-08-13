#pragma once

#include <cstdint>
#include "IpAddress.h"

class SocketAddress {
private:
    IpAddress address;
    std::uint16_t port_{0};

public:
    SocketAddress() = delete;
    explicit SocketAddress(
        const IpAddress &address,
        const std::uint16_t port
        ) : address{address}, port_{port} {}

    [[nodiscard]]
    const IpAddress& ip_address() const noexcept;

    [[nodiscard]]
    std::uint16_t port() const noexcept;

    [[nodiscard]]
    sockaddr_storage to_sockaddr() const noexcept;

    [[nodiscard]]
    static SocketAddress from_sockaddr(const sockaddr_storage &storage) noexcept;

    [[nodiscard]]
    socklen_t sockaddr_length() const noexcept;
};

