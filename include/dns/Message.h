#pragma once

#include <cstddef>
#include <vector>
#include <cstdint>
#include <optional>

#include "../../src/dns/Wire.h"
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
        std::vector<std::byte> rdata;    //rlength derived
    };

    class Message {
    private:
        std::uint16_t id_{0};
        std::uint16_t flags{0}; // QR:0 , OpCode: 1-4 , AA:5 , TC:6 , RD:7 , RA:8 , Z:9 , AD:10 , CD:11 , RCODE:12-15
        static constexpr std::uint16_t     QR_MASK{0x8000};
        static constexpr std::uint16_t OPCODE_MASK{0x7800};
        static constexpr std::uint16_t     AA_MASK{0x0400};
        static constexpr std::uint16_t     TC_MASK{0x0200};
        static constexpr std::uint16_t     RD_MASK{0x0100};
        static constexpr std::uint16_t     RA_MASK{0x0080};
        static constexpr std::uint16_t      Z_MASK{0x0040};
        static constexpr std::uint16_t     AD_MASK{0x0020};
        static constexpr std::uint16_t     CD_MASK{0x0010};
        static constexpr std::uint16_t   RCODE_MASK{0x000F};

        struct Counts{ std::uint16_t qd, an, ns, ar; };
        std::vector<Question> questions_;
        std::vector<ResourceRecord> answers_;
        std::vector<ResourceRecord> authorities_;
        std::vector<ResourceRecord> additionals_;

        Message() = default;

        static std::uint16_t createId();
        bool read_header(WireReader& reader, Counts& counts);
        bool read_question(WireReader& reader);
        std::optional<ResourceRecord> read_record(WireReader& reader);
        static std::optional<DomainName> read_name(WireReader& reader);
        
    public:
        [[nodiscard]]
        static Message query(const DomainName& name, RecordType type, RecordClass cls=RecordClass::IN);

        [[nodiscard]]
        static Message parse(std::span<const std::byte> buffer);

        [[nodiscard]]
        std::size_t encode(std::span<std::byte> buffer) const;

        [[nodiscard]]
        std::uint16_t id() const noexcept;

        [[nodiscard]]
        bool qr() const noexcept;

        [[nodiscard]]
        Opcode opcode() const noexcept;

        [[nodiscard]]
        bool aa() const noexcept;

        [[nodiscard]]
        bool tc() const noexcept;

        [[nodiscard]]
        bool rd() const noexcept;

        [[nodiscard]]
        bool ra() const noexcept;

        [[nodiscard]]
        bool z() const noexcept;

        [[nodiscard]]
        bool ad() const noexcept;

        [[nodiscard]]
        bool cd() const noexcept;

        [[nodiscard]]
        ResponseCode rcode() const noexcept;

        [[nodiscard]]
        std::span<const Question> questions() const noexcept;

        [[nodiscard]]
        std::span<const ResourceRecord> authorities() const noexcept;

        [[nodiscard]]
        std::span<const ResourceRecord> answers() const noexcept;

        [[nodiscard]]
        std::span<const ResourceRecord> additionals() const noexcept;
    };
}