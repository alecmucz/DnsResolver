#pragma once
#include <sys/socket.h>
#include <span>
#include <optional>
#include <system_error>
#include <stdexcept>
#include <cerrno>

#include "FileDescriptor.h"
#include "SocketAddress.h"

class DatagramSocket {
private:
    FileDescriptor fd;

public:
    DatagramSocket() = delete;

    explicit DatagramSocket(sa_family_t family)
        : fd{(socket(family, SOCK_DGRAM, 0))} {
        if ((family == AF_INET || family == AF_INET6) && fd.value() == -1) {
            throw std::system_error(errno, std::generic_category(), "DatagramSocket::DatagramSocket");
        }
        if (family != AF_INET && family != AF_INET6) {
            throw std::invalid_argument("DatagramSocket only supports IPv4 and IPv6");
        }
    }

    void bind(const SocketAddress &address) ;

    ssize_t send(std::span<const std::byte> data,
              const SocketAddress &address) const;

    ssize_t recv(std::span<std::byte> buffer,
                 std::optional<SocketAddress> &sender) const;

};
