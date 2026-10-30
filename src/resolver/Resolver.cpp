#include <socket/DatagramSocket.h>
#include <resolver/Resolver.h>

dns::Message dns::Resolver::resolve(const dns::DomainName& name, dns::RecordType type) {
    Message q = dns::Message::query(name,type);
    std::array<std::byte, 512> q_buffer{};
    std::size_t bytes_written = q.encode(q_buffer);
    sock.send(std::span(q_buffer).first(bytes_written), upstream);

    std::optional<SocketAddress> r_socket;
    std::array<std::byte, 512> r_buffer;
    auto n = sock.recv(r_buffer, r_socket, options.timeout);

    if (!n) {
        throw std::runtime_error("Failed to receive response");
    }

    Message r = Message::parse(std::span(r_buffer).first(*n));
    return r;
}
