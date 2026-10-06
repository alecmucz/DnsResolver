#pragma once

#include <cstddef>
#include <vector>
#include <cstdint>
#include "dns/DomainName.h"

namespace dns {

    enum class Opcode {
        Query  = 0, Notify = 4, Update = 5
    };

    enum class RecordType : std::uint16_t {
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
        RecordType type;
        RecordClass class_{RecordClass::IN};
    };

    struct ResourceRecord {
        DomainName name;
        RecordType type;
        RecordClass class_{RecordClass::IN};
        std::uint32_t ttl{};     // RFC:2181 Sec.8 TTL; If MSB = 1 then value = 0
        std::vector<std::byte> data;
    };

    class Message {
    private:
        friend class Parser;
        std::uint16_t id_{0};
        std::uint16_t flags{0}; // QR:0 , OpCode: 1-4 , AA:5 , TC:6 , RD:7 , RA:8 , Z:9 , AD:10 , CD:11 , RCODE:12-15

        std::vector<Question> questions;
        std::vector<ResourceRecord> answers;
        std::vector<ResourceRecord> authorities;
        std::vector<ResourceRecord> additionals;

        static constexpr std::uint16_t RD_MASK{0x0100};

        Message() = default;

        static std::uint16_t createId();
        
    public:
        [[nodiscard]]
        static Message query(const DomainName& name, RecordType type, RecordClass cls);

        [[nodiscard]]
        std::uint16_t id() const noexcept;

        [[nodiscard]]
        bool recursion_desired() const noexcept;
    };

}