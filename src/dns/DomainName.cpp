#include "dns/DomainName.h"
#include <algorithm>



dns::DomainName::DomainName(std::string_view n) {

}

dns::DomainName::DomainName(std::span<const std::byte> bytes) {
    for (size_t i = 0; i < bytes.size();) {
        uint8_t label_length = std::to_integer<uint8_t>(bytes[i]);

        if ((label_length & 0b1100'0000) != 0) {
            throw std::invalid_argument("Domain Name Requires Decompression");
        }

        if (label_length + i + 1 > bytes.size()) {
            throw std::invalid_argument("Domain Name Label Exceeds Input Bounds");
        }

        if (label_length == 0) {
            i += 1;
            if (i > name.size()) {
                throw std::invalid_argument("Domain Name Exceeds 255 Bytes");
            }
            std::ranges::copy(bytes.subspan(0, i), name.begin());
            length = static_cast<uint8_t>(i);
            return;
        }
        i += label_length + 1;
    }
    throw std::invalid_argument("Incomplete Domain Name Wire Data");
}

bool dns::DomainName::operator==(const DomainName& other) const noexcept {
    if (length != other.length) {
        return false;
    }
    for (size_t i = 0; i < length; ++i) {
        auto check = other.name[i];
        auto og = this->name[i];
        if (check >= std::byte{'A'} && check <= std::byte{'Z'}) {
            check |= std::byte{0x20};
        }
        if (og >= std::byte{'A'} && og <= std::byte{'Z'}) {
            og |= std::byte{0x20};
        }
        if (og != check) {
            return false;
        }
    }
    return true;
}