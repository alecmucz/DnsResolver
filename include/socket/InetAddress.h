#pragma once

#include <variant>
#include <netinet/in.h>

class InetAddress {
private:
    std::variant<in_addr, in6_addr> address;

public:

};