#include <arpa/inet.h>
#include <string>
#include "socket/IpAddress.h"

const in_addr& IpAddress::ipv4() const noexcept {
    return std::get<in_addr>(address);
}

const in6_addr& IpAddress::ipv6() const noexcept {
    return std::get<in6_addr>(address);
}

sa_family_t IpAddress::family() const noexcept {
    return std::holds_alternative<in_addr>(address)
            ? AF_INET : AF_INET6;
}

// ToDo: optional -> expected for error handling
std::optional<IpAddress> IpAddress::from_string(std::string_view address) {
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
        inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
    } else {
        const auto &addr6 = std::get<in6_addr>(address);
        inet_ntop(AF_INET6, &addr6, buffer, INET6_ADDRSTRLEN);
    }

    return std::string{buffer};
}

