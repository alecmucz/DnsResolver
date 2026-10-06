#include "dns/Message.h"

#include <random>

std::uint16_t dns::Message::createId() {
    static std::random_device rd{};
    std::uniform_int_distribution<std::uint16_t> dis;
    return dis(rd);
}

dns::Message dns::Message::query(const DomainName &name, RecordType type, RecordClass cls = RecordClass::IN) {
    Message m{};
    m.id_ = createId();
    m.flags |= RD_MASK;
    m.questions.emplace_back(name, type, cls);
    return m;
}

std::uint16_t dns::Message::id() const noexcept {
    return id_;
}

bool dns::Message::recursion_desired() const noexcept {
    return flags & RD_MASK;
}