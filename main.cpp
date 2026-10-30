#include <cstddef>
#include <cstdio>
#include <exception>
#include <print>
#include <string_view>

#include "resolver/Resolver.h"
#include "socket/IpAddress.h"
#include "socket/SocketAddress.h"

namespace {
std::string_view rcode_name(dns::ResponseCode rc) {
    switch (rc) {
        case dns::ResponseCode::NoError:           return "NOERROR";
        case dns::ResponseCode::FormatError:       return "FORMERR";
        case dns::ResponseCode::ServerFailure:     return "SERVFAIL";
        case dns::ResponseCode::NonExistentDomain: return "NXDOMAIN";
        case dns::ResponseCode::NotImplemented:    return "NOTIMP";
        case dns::ResponseCode::Refused:           return "REFUSED";
    }
    return "UNKNOWN";
}

std::string_view type_name(dns::RecordType t) {
    switch (t) {
        case dns::RecordType::A:     return "A";
        case dns::RecordType::NS:    return "NS";
        case dns::RecordType::CNAME: return "CNAME";
        case dns::RecordType::SOA:   return "SOA";
        case dns::RecordType::MX:    return "MX";
        case dns::RecordType::TXT:   return "TXT";
        case dns::RecordType::AAAA:  return "AAAA";
        case dns::RecordType::SRV:   return "SRV";
        default:                     return "OTHER";
    }
}
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::println(stderr, "usage: {} <name> [server]", argv[0]);
        return 1;
    }

    const auto server = IpAddress::parse(argc == 3 ? argv[2] : "1.1.1.1");
    if (!server) {
        std::println(stderr, "invalid server address: {}", argv[2]);
        return 1;
    }

    try {
        dns::Resolver resolver{SocketAddress{*server, 53}, {}};
        const auto reply = resolver.resolve(dns::DomainName{argv[1]}, dns::RecordType::A);

        if (reply.rcode() != dns::ResponseCode::NoError) {
            std::println("{}", rcode_name(reply.rcode()));
            return 0;
        }
        if (reply.answers().empty()) {
            std::println("NODATA");
            return 0;
        }

        for (const auto& rr : reply.answers()) {
            if (rr.type == dns::RecordType::A && rr.rdata.size() == 4) {
                std::println("{}  {}  A  {}.{}.{}.{}", rr.name.toString(), rr.ttl,
                             std::to_integer<unsigned>(rr.rdata[0]),
                             std::to_integer<unsigned>(rr.rdata[1]),
                             std::to_integer<unsigned>(rr.rdata[2]),
                             std::to_integer<unsigned>(rr.rdata[3]));
            } else {
                std::println("{}  {}  {}  ({} bytes)", rr.name.toString(), rr.ttl,
                             type_name(rr.type), rr.rdata.size());
            }
        }
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}