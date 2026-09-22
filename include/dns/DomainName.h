#pragma once

#include <string_view>
#include <array>
#include <cstddef>
#include <span>

namespace dns { class DomainName {
private:
    std::array<std::byte, 255> name;
    std::uint8_t length;

public:
    explicit DomainName(std::string_view n);
    explicit DomainName(std::span<const std::byte> bytes);

    bool operator==(const DomainName& other) const noexcept;
};
}