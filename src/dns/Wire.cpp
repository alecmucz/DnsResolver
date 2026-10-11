#include "Wire.h"

#include <algorithm>
#include <array>

dns::WireWriter::WireWriter(std::span<std::byte> buffer)
    : buffer{buffer}, pos{0} {}

bool dns::WireWriter::write_bytes(std::span<const std::byte> src) {
    if (src.size() > buffer.size() - pos)
        return false;
    std::ranges::copy_n(src.begin(), src.size(), buffer.begin() + pos);
    pos += src.size();
    return true;
}

bool dns::WireWriter::write_u8(std::uint8_t n) {
    std::array<std::byte, 1> bytes{std::byte{n}};
    return write_bytes(bytes);

}

bool dns::WireWriter::write_u16(std::uint16_t n) {
    std::array<std::byte, 2> bytes{static_cast<std::byte>(n >> 8), static_cast<std::byte>(n)};
    return write_bytes(bytes);
}

std::size_t dns::WireWriter::bytes_written() const{
    return pos;
}