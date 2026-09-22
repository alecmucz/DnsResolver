#pragma once

#include <cstddef>
#include <vector>

#include "dns/DomainName.h"

namespace dns {

    enum class Opcode {
        Query  = 0, Notify = 4, Update = 5
    };

    enum class ResourceRecordType : std::uint16_t {
        A   = 1,  NS   = 2,  CNAME = 5,  SOA = 6, MX  = 15,
        TXT = 16, AAAA = 28, SRV   = 33, ANY = 255
    };

    enum class RecordClass : std::uint16_t {
        IN  = 1, NONE = 254, ANY = 255
    };

    enum class ResponseCode {
        NoError           = 0, FormatError    = 1, ServerFailure = 2,
        NonExistentDomain = 3, NotImplemented = 4, Refused       = 5
    };

    struct Question {
        DomainName qname;
        ResourceRecordType type;
        RecordClass class_{RecordClass::IN};
    };

    struct ResourceRecord {
        DomainName name;
        ResourceRecordType type;
        RecordClass class_{RecordClass::IN};
        std::int32_t ttl{};     // RFC:1035 2.3.4. TTL -  Positive values of a signed 32 bit integer
        std::vector<std::byte> data;
    };

    class Message {
    private:
        std::uint16_t id{0};
        std::uint16_t flags{0}; // QR:0 , OpCode: 1-4 , AA:5 , TC:6 , RD:7 , RA:8 , Z:9 , AD:10 , CD:11 , RCODE:12-15

        std::vector<Question> questions;
        std::vector<ResourceRecord> answers;
        std::vector<ResourceRecord> authorities;
        std::vector<ResourceRecord> additionals;
        
    public:

    };

}