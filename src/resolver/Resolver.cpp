#include <socket/DatagramSocket.h>
#include <resolver/Resolver.h>

dns::Message dns::Resolver::resolve(const dns::DomainName& name, dns::RecordType type) {
    Message query = Message::query(name,type);
    std::optional<SocketAddress> r_addr;
    std::array<std::byte, 512> q_buffer{};
    std::array<std::byte, 512> r_buffer{};
    std::size_t bytes_written = query.encode(q_buffer);

    for (int i = 0; i <= options.retries; i++) {
        auto _ = sock.send(std::span(q_buffer).first(bytes_written), upstream);
        auto deadline = std::chrono::steady_clock::now() + options.timeout;
        for (;;) {
            auto remain = std::chrono::ceil<std::chrono::milliseconds>
                                    (deadline - std::chrono::steady_clock::now());
            if (remain <= std::chrono::milliseconds::zero()) break;

            auto n = sock.recv(r_buffer, r_addr, remain);
            if (!n) {   // timeout
                break;
            }

            // ToDo: Clean Up Try/Catch with proper error handling
            if (r_addr != upstream) continue;
            std::optional<Message> response;
            try {
                response = Message::parse(std::span(r_buffer).first(*n));
            } catch (const std::exception&) { continue;}    // malformed datagram

            if (response->id() != query.id()) continue;
            if (!response.value().qr()) continue;

            const auto questions = response->questions();
            if (questions.empty()
                || !(questions[0].qname == name)
                || questions[0].type != type
                || questions[0].class_ != RecordClass::IN) continue;

                if (response->tc()) {
                    throw std::runtime_error("DNS Resolver: Truncated Response");
                }
                return *response;
        }
    }
    throw std::runtime_error("DNS Resolver: Timed Out");
}