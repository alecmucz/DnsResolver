#include <iostream>
#include <print>
#include <optional>
#include <string_view>
#include <cstddef>

#include <arpa/inet.h>

#include "dns/Message.h"
#include "socket/IpAddress.h"
#include "socket/DatagramSocket.h"
#include "socket/SocketAddress.h"

int main() {
    dns::Message query = dns::Message::query(dns::DomainName{"www.example.com"}, dns::RecordType::A);

    std::array<std::byte, 512> buffer;
    const size_t n = query.encode(buffer);

    std::println("encoded {} bytes, id = {:04x}", n, query.id());

    for (const std::byte b : std::span{buffer}.first(n)) {
        std::print("{:02x} ", std::to_integer<unsigned>(b));
    }
    std::print("\n");

    const SocketAddress upstream{*IpAddress::parse("1.1.1.1"), 53};
    const DatagramSocket sock{AF_INET};
    sock.send(std::span{buffer}.first(n), upstream);

    std::array<std::byte, 512> reply;
    std::optional<SocketAddress> sender;
    using namespace std::chrono_literals;
    auto m = sock.recv(reply, sender, 2s);
    if (m) std::println("received {} bytes", *m);
    else   std::println("timed out");

    return 0;
}