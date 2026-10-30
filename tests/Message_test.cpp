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

TEST_CASE("DNS Message Parsing") {
    using namespace std::string_view_literals;
    constexpr auto response =
        // Header
        "\022\064"              // ID = 0x1234
        "\201\200"              // flags: QR, RD, RA
        "\000\001"              // QDCOUNT = 1
        "\000\002"              // ANCOUNT = 2
        "\000\000"              // NSCOUNT = 0
        "\000\000"              // ARCOUNT = 0
        // Question
        "\003www\007example\003com\000"
        "\000\001"              // QTYPE  = A
        "\000\001"              // QCLASS = IN
        // Answer 1
        "\300\014"              // NAME   = pointer to offset 12
        "\000\001"              // TYPE   = A
        "\000\001"              // CLASS  = IN
        "\000\000\016\020"      // TTL    = 3600
        "\000\004"              // RDLENGTH = 4
        "\135\270\330\042"      // 93.184.216.34
        // Answer 2
        "\300\014"
        "\000\001"
        "\000\001"
        "\000\000\000\074"      // TTL    = 60
        "\000\004"
        "\001\002\003\004"sv;   // 1.2.3.4

    auto bytes = std::as_bytes(std::span{response});
    auto m = dns::Message::parse(bytes);

    REQUIRE(m.id() == 0x1234);
    REQUIRE(m.qr() == true);
    REQUIRE(m.rd() == true);
    REQUIRE(m.ra() == true);
    REQUIRE(m.aa() == false);
    REQUIRE(m.tc() == false);
    REQUIRE(m.rcode() == dns::ResponseCode::NoError);
    REQUIRE(m.questions().size() == 1);
    REQUIRE(m.answers().size() == 2);
    REQUIRE(m.authorities().size() == 0);
    REQUIRE(m.additionals().size() == 0);
    
    const auto& a1 = m.answers()[0];
    const auto& a2 = m.answers()[1];

    REQUIRE(a1.type == dns::RecordType::A);
    REQUIRE(a1.ttl == 3600);
    REQUIRE(a1.rdata.size() == 4);
    REQUIRE(a1.rdata == std::vector{std::byte{93}, std::byte{184}, std::byte{216}, std::byte{34}});

    REQUIRE(a2.ttl == 60);
    REQUIRE(a2.rdata == std::vector{std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}});

    REQUIRE(m.questions()[0].qname.toString() == "www.example.com");
    REQUIRE(m.answers()[0].name.toString() == "www.example.com");   // via \300\014 pointer
    REQUIRE(m.answers()[1].name.toString() == "www.example.com");

    REQUIRE_THROWS(dns::Message::parse(bytes.first(0)));    // empty
    REQUIRE_THROWS(dns::Message::parse(bytes.first(8)));    // short header
    REQUIRE_THROWS(dns::Message::parse(bytes.first(40)));   // inside answer 1
}