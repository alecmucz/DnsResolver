#pragma once
#include <sys/socket.h>
#include <span>

#include "FileDescriptor.h"
#include "SocketAddress.h"

class DatagramSocket {
private:
    FileDescriptor fd;

public:
    DatagramSocket() = delete;

    explicit DatagramSocket(sa_family_t family) :
                            fd{socket(family, SOCK_DGRAM, 0)} {}

    void bind(const SocketAddress &address) ;

    void send(std::span<const std::byte> data,
              const SocketAddress &address) const;

};
