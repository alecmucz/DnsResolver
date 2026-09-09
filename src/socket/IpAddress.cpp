#include <arpa/inet.h>
#include <string>
#include <system_error>
#include "socket/IpAddress.h"

const in_addr& IpAddress::ipv4() const {
    return std::get<in_addr>(address);
}

const in6_addr& IpAddress::ipv6() const {
    return std::get<in6_addr>(address);
}

sa_family_t IpAddress::family() const noexcept {
    return std::holds_alternative<in_addr>(address)
            ? AF_INET : AF_INET6;
}

std::optional<IpAddress> IpAddress::parse(std::string_view address) {
    std::string str_address{address};

    if (in_addr addr{}; inet_pton(AF_INET, str_address.c_str(), &addr) == 1) {
        return IpAddress(addr);
    } else if (in6_addr addr6{}; inet_pton(AF_INET6, str_address.c_str(), &addr6) == 1) {
        return IpAddress(addr6);
    }
    return std::nullopt;
    }

std::string IpAddress::to_string() const {
    char buffer[INET6_ADDRSTRLEN];

    if (family() == AF_INET) {
        const auto &addr = std::get<in_addr>(address);
        if (!inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN)) {
            throw std::system_error{errno, std::system_category(), "IpAddress: Network -> Presentation Conversion Failed"};
        }
    } else {
        const auto &addr6 = std::get<in6_addr>(address);
        if (!inet_ntop(AF_INET6, &addr6, buffer, INET6_ADDRSTRLEN)) {
            throw std::system_error{errno, std::system_category(), "IpAddress: Network -> Presentation Conversion Failed"};
        }
    }
    return std::string{buffer};
}
