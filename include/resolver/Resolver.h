#pragma once

#include <chrono>
#include <socket/SocketAddress.h>
#include <socket/DatagramSocket.h>
#include <dns/Message.h>

namespace dns {
struct ResolverOptions {
    std::chrono::milliseconds timeout{std::chrono::seconds(5)};
    int retries{2};
};
class Resolver {
private:
    DatagramSocket sock;
    SocketAddress upstream;
    ResolverOptions options;
public:
    explicit Resolver(const SocketAddress& upstream, ResolverOptions opts)
                        : sock(DatagramSocket(upstream.ip_address().family())),
                          upstream(upstream), options(opts) {}

    [[nodiscard]]
    Message resolve(const DomainName& name, RecordType type);
};
}