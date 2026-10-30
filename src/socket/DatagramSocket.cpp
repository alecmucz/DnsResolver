#include <chrono>
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

std::optional<std::size_t> DatagramSocket::recv(std::span<std::byte> buffer, std::optional<SocketAddress> &sender, std::chrono::milliseconds timeout) const {
    sender.reset();
    sockaddr_storage storage{};
    socklen_t addr_len = sizeof(storage);

    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    const auto usecs = std::chrono::duration_cast<std::chrono::microseconds>(timeout - secs);
    timeval tv{};
    tv.tv_sec  = static_cast<decltype(tv.tv_sec)>(secs.count());
    tv.tv_usec = static_cast<decltype(tv.tv_usec)>(usecs.count());

    if (::setsockopt(fd.value(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
        throw std::system_error(errno, std::generic_category(), "DatagramSocket::recv() setsockopt");
    }

    const auto bytes = ::recvfrom(
        fd.value(),
        buffer.data(),
        buffer.size(),
        0,
        reinterpret_cast<sockaddr*>(&storage),
        &addr_len);

    if (bytes >= 0) {
        sender = SocketAddress::from_sockaddr(storage);
        return static_cast<std::size_t>(bytes);
    } if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return std::nullopt;
    } else {
        throw std::system_error(errno, std::generic_category(), "DatagramSocket::recv()");
    }

    return bytes;
}
