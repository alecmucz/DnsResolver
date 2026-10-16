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
    bool write_bytes(std::span<const std::byte> src);

    [[nodiscard]]
    bool write_u8(std::uint8_t n);

    [[nodiscard]]
    bool write_u16(std::uint16_t n);

    [[nodiscard]]
    std::size_t bytes_written() const;

    WireWriter(const WireWriter&) = delete;
    WireWriter& operator=(const WireWriter&) = delete;

};

class WireReader {
private:
    std::span<const std::byte> buffer{};
    std::size_t pos{0};

public:
    explicit WireReader(std::span<const std::byte> buffer);

    [[nodiscard]]
    bool read_bytes(std::span<std::byte> dst);

    [[nodiscard]]
    bool read_u8(std::uint8_t& n);

    [[nodiscard]]
    bool read_u16(std::uint16_t& n);

    [[nodiscard]]
    bool read_u32(std::uint32_t& n);

    [[nodiscard]]
    bool seek(std::size_t offset);

    [[nodiscard]]
    std::size_t offset() const;

    [[nodiscard]]
    std::size_t remaining() const;

    WireReader(const WireReader&) = delete;
    WireReader& operator=(const WireReader&) = delete;

};
};

