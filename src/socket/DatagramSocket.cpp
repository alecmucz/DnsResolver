#include <socket/DatagramSocket.h>

void DatagramSocket::bind(const SocketAddress &address) {
    const auto storage = address.to_sockaddr();
    if (::bind(
        fd.value(),
        reinterpret_cast<const sockaddr*>(&storage),
        address.sockaddr_length()
        ) == -1) {
        throw std::system_error(errno, std::generic_category(), "DatagramSocket::bind()");
    }
}

ssize_t DatagramSocket::send(std::span<const std::byte> data, const SocketAddress &address) const {
    const auto storage = address.to_sockaddr();

    const auto bytes = ::sendto(
        fd.value(),
        data.data(),
        data.size(),
        0,
        reinterpret_cast<const sockaddr*>(&storage),
        address.sockaddr_length()
    );

    if (bytes == -1) {
        throw std::system_error(errno, std::generic_category(), "DatagramSocket::send()");
    }

    return bytes;
}

ssize_t DatagramSocket::recv(std::span<std::byte> buffer, std::optional<SocketAddress> &sender) const {
    sender.reset();
    sockaddr_storage storage{};
    socklen_t addr_len = sizeof(storage);

    const auto bytes = ::recvfrom(
        fd.value(),
        buffer.data(),
        buffer.size(),
        0,
        reinterpret_cast<sockaddr*>(&storage),
        &addr_len);

    if (bytes >= 0) {
        sender = SocketAddress::from_sockaddr(storage);
    } else {
        throw std::system_error(errno, std::generic_category(), "DatagramSocket::recv()");
    }


    return bytes;
}
