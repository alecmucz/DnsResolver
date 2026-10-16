#include "dns/Message.h"
#include "Wire.h"
#include <random>
#include <utility>

std::uint16_t dns::Message::createId() {
    static std::random_device rd{};
    std::uniform_int_distribution<std::uint16_t> dis;
    return dis(rd);
}

dns::Message dns::Message::query(const DomainName &name, RecordType type, RecordClass cls) {
    Message m{};
    m.id_ = createId();
    m.flags |= RD_MASK;
    m.questions.emplace_back(name, type, cls);
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