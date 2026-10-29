#include <catch2/catch_test_macros.hpp>
#include "dns/DomainName.h"

TEST_CASE("Domain Name Tests") {
    SECTION("Domain Name Creation (www.example.com)") {
        dns::DomainName domainName{"www.example.com"};
        REQUIRE(domainName.toString() == "www.example.com");
    }
    SECTION("Domain Name Creation (example.com)") {
        dns::DomainName domainName{"example.com"};
        REQUIRE(domainName.toString() == "example.com");
    }
    SECTION("Domain Name Bytes") {
        using namespace std::string_view_literals;
        dns::DomainName domainName{"www.example.com"};
        auto b = domainName.bytes();
        std::string_view actual{reinterpret_cast<const char*>(b.data()), b.size()};
        REQUIRE(actual == "\003www\007example\003com\000"sv);
    }
    SECTION("Domain Name Equality") {
        dns::DomainName domainName1{"www.example.com"};
        dns::DomainName domainName2{"www.example.com"};
        REQUIRE(domainName1 == domainName2);
    }
    SECTION("Invalid Domain Name") {
        REQUIRE_THROWS_AS(dns::DomainName{""}, std::invalid_argument);
    }
}