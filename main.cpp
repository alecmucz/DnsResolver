#include <iostream>
#include <arpa/inet.h>

#include "include/socket/IpAddress.h"
#include "socket/DatagramSocket.h"
#include "socket/SocketAddress.h"

int main() {
    auto ip = IpAddress::from_string("127.0.0.1");

    SocketAddress addr{*ip, 9000};
    DatagramSocket socket{AF_INET};

    std::string message{"UDP Message"};

    std::span<const char> data{message.data(), message.size()};

    socket.send(std::as_bytes(data), addr);

    return 0;
}