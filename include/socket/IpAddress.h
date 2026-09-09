#pragma once

#include <optional>
#include <string_view>
#include <variant>
#include <netinet/in.h>

class IpAddress {
private:
    friend class SocketAddress;
    std::variant<in_addr, in6_addr> address;

    explicit IpAddress(in_addr address) : address(address) {}
    explicit IpAddress(in6_addr address) : address(address) {}

    [[nodiscard]] const in_addr& ipv4() const;
    [[nodiscard]] const in6_addr& ipv6() const;

public:
    IpAddress() = delete;

    [[nodiscard]] sa_family_t family() const noexcept;

    static std::optional<IpAddress> parse(std::string_view address);

    [[nodiscard]]
    std::string to_string() const;
};