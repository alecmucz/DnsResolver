#pragma once

#include <optional>
#include <string_view>
#include <variant>
#include <netinet/in.h>

class IpAddress {
private:
    std::variant<in_addr, in6_addr> address;

public:
    IpAddress() = delete;
    explicit IpAddress(in_addr address) : address(address) {}
    explicit IpAddress(in6_addr address) : address(address) {}

    [[nodiscard]] sa_family_t family() const noexcept;

    static std::optional<IpAddress> from_string(std::string_view address);

    [[nodiscard]]
    std::string to_string() const;

};