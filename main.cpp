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
    auto ip = IpAddress::parse("127.0.0.1");

    if (!ip) {
        std::println("Invalid IP address");
        return 1;
    }

    SocketAddress address{*ip, 9000};
    DatagramSocket server{AF_INET};

    server.bind(address);

    std::println(
        "Server Listening on {}:{}\n\n",
        address.ip_address().to_string(),
        address.port()
    );

    while (true) {
        std::byte buffer[1024];
        std::optional<SocketAddress> sender{};

        const auto bytes_received = server.recv(buffer, sender);

        if (bytes_received < 0) {
            std::println("recv() failed");
            continue;
        }

        if (!sender) {
            std::println("recv() returned without sender");
            continue;
        }

        const std::string_view data{
            reinterpret_cast<const char*>(buffer),
            static_cast<std::size_t>(bytes_received)
        };

        std::println(
            "Received {} bytes from {}:{} \nMessage: {}",
            bytes_received,
            sender->ip_address().to_string(),
            sender->port(),
            data
        );

        std::print("Server> ");
        std::string reply;

        std::getline(std::cin, reply);

        if (reply.empty()) {
            std::println("Empty reply");
            continue;
        }

        const auto reply_data = std::as_bytes(std::span{reply});

        const auto sent = server.send(reply_data, *sender);

        if (sent < 0) {
            std::println("send() failed");
            continue;
        }
        std::println("Sent {} bytes", sent);
    }
}