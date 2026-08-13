#include <iostream>
#include <print>
#include <optional>
#include <string_view>
#include <cstddef>

#include <arpa/inet.h>

#include "socket/IpAddress.h"
#include "socket/DatagramSocket.h"
#include "socket/SocketAddress.h"

int main() {
    auto ip = IpAddress::from_string("127.0.0.1");

    SocketAddress address{*ip, 9000};
    DatagramSocket server{AF_INET};

    server.bind(address);

    std::println(
        "Listening on {}:{}",
        address.ip_address().to_string(),
        address.port()
    );

    while (true) {
        std::byte buffer[1024];
        std::optional<SocketAddress> sender{};

        const auto bytes_received = server.recv(buffer, sender);

        if (bytes_received < 0) {
            std::println("recv failed");
            continue;
        }

        const auto size =
            static_cast<std::size_t>(bytes_received);

        const std::string_view data{
            reinterpret_cast<const char*>(buffer),
            size
        };

        std::println(
            "Received {} bytes from {}:{} -> {}",
            bytes_received,
            sender->ip_address().to_string(),
            sender->port(),
            data
        );

        server.send(
            std::span<const std::byte>{buffer, size},
            *sender
        );


    }
}