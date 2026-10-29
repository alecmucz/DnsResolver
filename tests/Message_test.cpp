#include <catch2/catch_test_macros.hpp>
#include "dns/Message.h"

TEST_CASE("Message Test") {
    SECTION("DNS Message Query Construction") {
        dns::Message q = dns::Message::query(dns::DomainName{"www.example.com"},
                                             dns::RecordType::A,
                                        dns::RecordClass::IN);
        REQUIRE(q.qr() == false);
        REQUIRE(q.rd() == true);
        REQUIRE(q.opcode() == dns::Opcode::Query);

        auto qs = q.questions();
        REQUIRE(qs.size() == 1);
        REQUIRE(qs[0].qname == dns::DomainName{"www.example.com"});
    }
}

TEST_CASE("DNS Message Encoding") {
    using namespace std::string_view_literals;
    dns::Message m = dns::Message::query(dns::DomainName{"www.example.com"},
                                         dns::RecordType::A,
                                         dns::RecordClass::IN);
    std::array<std::byte, 512> buffer{};
    std::size_t bytes_written = m.encode(buffer);
    REQUIRE(bytes_written == 33);
    std::span<const std::byte> out{buffer.data(), bytes_written};
    std::string_view bytes{reinterpret_cast<const char*>(out.data()), out.size()};

    const auto id = static_cast<std::uint16_t>(
                    (std::to_integer<std::uint16_t>(out[0]) << 8) | std::to_integer<std::uint16_t>(out[1]));
    REQUIRE(id == m.id());
    REQUIRE(bytes.substr(2, 10) ==
            "\x01\x00"          // flags: RD set
            "\x00\x01"          // QDCOUNT = 1
            "\x00\x00"          // ANCOUNT = 0
            "\x00\x00"          // NSCOUNT = 0
            "\x00\x00"sv);      // ARCOUNT = 0

    REQUIRE(bytes.substr(12) ==
            "\003www\007example\003com\000"
            "\x00\x01"          // QTYPE  = A
            "\x00\x01"sv);      // QCLASS = IN
}
