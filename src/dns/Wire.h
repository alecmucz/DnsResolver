#pragma once
#include <cstddef>
#include <cstdint>
#include <span>


namespace dns {
class WireWriter {
private:
    std::span<std::byte> buffer{};
    std::size_t pos{0};

public:
    explicit WireWriter(std::span<std::byte> buffer);

    [[nodiscard]]
    bool write_u8(std::uint8_t n);

    [[nodiscard]]
    bool write_u16(std::uint16_t n);

    [[nodiscard]]
    bool write_bytes(std::span<const std::byte> src);

    [[nodiscard]]
    std::size_t bytes_written() const;

    WireWriter(const WireWriter&) = delete;
    WireWriter& operator=(const WireWriter&) = delete;

};
};

