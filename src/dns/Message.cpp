#include "dns/Message.h"
#include "Wire.h"
#include <random>
#include <utility>
#include <sys/sysctl.h>
#include <stdexcept>

std::uint16_t dns::Message::createId() {
    static std::random_device rd{};
    std::uniform_int_distribution<std::uint16_t> dis;
    return dis(rd);
}

bool dns::Message::read_header(WireReader& reader, Counts& counts) {
    return reader.read_u16(id_)
        && reader.read_u16(flags)
        && reader.read_u16(counts.qd)
        && reader.read_u16(counts.an)
        && reader.read_u16(counts.ns)
        && reader.read_u16(counts.ar);
}

// ToDo: Pointer Loop Guard
std::optional<dns::DomainName> dns::Message::read_name(WireReader& reader){
    std::array<std::byte, 255> name_buffer{};
    std::size_t pos{};
    std::size_t prev_pos{};
    bool jump{false};

    while (1) {
        std::uint8_t label{};

        if (!reader.read_u8(label)) {
            return std::nullopt;
        }

        if ((label & 0b1100'0000) == 0b1100'0000) {     // Pointer
            std::uint16_t offset{label};
            offset = (offset & 0b0011'1111) << 8;
            if (!reader.read_u8(label)) {
                return std::nullopt;
            }
            offset = offset | label;

            if (!jump) {
                prev_pos = reader.offset();
                jump = true;
            }

            if (!reader.seek(offset)) {
                return std::nullopt;
            }
            continue;
        }


        if ((label & 0b1100'0000) == 0b0000'0000) {     // Label
            if (pos + label  + 1 > name_buffer.size())
                return std::nullopt;

            name_buffer[pos] = static_cast<std::byte>(label);
            pos += 1;

            if (label == 0)
                break;

            if (!reader.read_bytes(std::span(name_buffer).subspan(pos, label)))
                return std::nullopt;

            pos += label;
        }

        if ((label & 0b1000'0000) == 0b1000'0000    // Reserved (RFC 6891)
            || (label & 0b0100'0000) == 0b0100'0000) { // Extended Label (RFC 2673 & RFC 6891)
            return std::nullopt;
        }
    }
    if (jump) if (!reader.seek(prev_pos)) return std::nullopt;
    return DomainName{std::span(name_buffer).first(pos)};
}

dns::Message dns::Message::query(const DomainName &name, RecordType type, RecordClass cls) {
    Message m{};
    m.id_ = createId();
    m.flags |= RD_MASK;
    m.questions.emplace_back(name, type, cls);
    return m;
}

dns::Message dns::Message::parse(std::span<const std::byte> buffer) {
    WireReader reader{buffer};
    Message m{};
    Counts m_counts{};

    if (!(m.read_header(reader, m_counts)))
        throw std::runtime_error{"Invalid DNS message"};



    return m;
}

std::size_t dns::Message::encode(std::span<std::byte> buffer) const {
    WireWriter writer{buffer};

    uint16_t qdcount = static_cast<uint16_t>(questions.size());

    if (!(writer.write_u16(id_)
        && writer.write_u16(flags)
        && writer.write_u16(qdcount)
        && writer.write_u16(0)
        && writer.write_u16(0)
        && writer.write_u16(0))) {
        return 0;}

    for (const auto &q : questions) {
        if (!(writer.write_bytes(q.qname.bytes())
        &&  writer.write_u16(std::to_underlying(q.type))
        &&  writer.write_u16(std::to_underlying(q.class_)))){
        return 0;}
    }

    return writer.bytes_written();
}

std::uint16_t dns::Message::id() const noexcept {
    return id_;
}

bool dns::Message::rd() const noexcept {
    return flags & RD_MASK;
}

bool dns::Message::qr() const noexcept {
    return flags & QR_MASK;
}