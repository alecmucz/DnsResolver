#include <socket/DatagramSocket.h>

void DatagramSocket::bind(const SocketAddress &address) {
    const auto storage = address.to_sockaddr();
    ::bind(
        fd.value(),
        reinterpret_cast<const sockaddr*>(&storage),
        address.sockaddr_length()
        );
}

void DatagramSocket::send(std::span<const std::byte> data, const SocketAddress &address) const {
    const auto storage = address.to_sockaddr();
    ::sendto(
        fd.value(),
        data.data(),
        data.size(),
        0,
        reinterpret_cast<const sockaddr*>(&storage),
        address.sockaddr_length()
    );
}
