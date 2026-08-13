#include "socket/SocketAddress.h"

const IpAddress& SocketAddress::ip_address() const noexcept {
    return address;
}

std::uint16_t SocketAddress::port() const noexcept {
    return port_;
}

sockaddr_storage SocketAddress::to_sockaddr() const noexcept {
    sockaddr_storage storage{};

    if (address.family() == AF_INET) {
        sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port_),
            .sin_addr = address.ipv4(),
        };
        std::memcpy(&storage, &addr, sizeof(addr));

    } else {
        sockaddr_in6 addr = {
            .sin6_family = AF_INET6,
            .sin6_port = htons(port_),
            .sin6_addr = address.ipv6(),
        };
        std::memcpy(&storage, &addr, sizeof(addr));
    }
    return storage;
}

SocketAddress SocketAddress::from_sockaddr(const sockaddr_storage &storage) noexcept {

    if (storage.ss_family == AF_INET) {
        const auto *addr = reinterpret_cast<const sockaddr_in*>(&storage);
        return SocketAddress{
            IpAddress(addr->sin_addr),
            ntohs(addr->sin_port)
        };
    } else if (storage.ss_family == AF_INET6) {
        const auto *addr = reinterpret_cast<const sockaddr_in6*>(&storage);
        return SocketAddress{
            IpAddress(addr->sin6_addr),
            ntohs(addr->sin6_port)
        };
    }
    std::unreachable();
}

socklen_t SocketAddress::sockaddr_length() const noexcept {
    if (address.family() == AF_INET) {
        return sizeof(sockaddr_in);
    }
    return sizeof(sockaddr_in6);
}