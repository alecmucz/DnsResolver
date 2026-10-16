#include "Wire.h"

#include <algorithm>
#include <array>

// WireWriter
dns::WireWriter::WireWriter(std::span<std::byte> buffer)
    : buffer{buffer}, pos{0} {}

bool dns::WireWriter::write_bytes(std::span<const std::byte> src) {
    if (src.size() > buffer.size() - pos) {return false;}
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

// WireReader
dns::WireReader::WireReader(std::span<const std::byte> buffer)
    : buffer{buffer}, pos{0} {}

bool dns::WireReader::read_bytes(std::span<std::byte> dst) {
    if (dst.size() > remaining())   return false;
    std::ranges::copy_n(buffer.begin() + pos, dst.size(), dst.begin());
    pos += dst.size();
    return true;
}

bool dns::WireReader::read_u8(std::uint8_t& n) {
    std::array<std::byte, 1> bytes{};
    if (!read_bytes(bytes)) {return false;}
    n = std::to_integer<std::uint8_t>(bytes[0]);
    return true;
}

bool dns::WireReader::read_u16(std::uint16_t& n) {
    std::array<std::byte, 2> bytes{};
    if (!read_bytes(bytes)) {return false;}
    n = std::to_integer<std::uint16_t>(bytes[0] << 8 | bytes[1]);
    return true;
}

bool dns::WireReader::read_u32(std::uint32_t& n) {
    std::array<std::byte, 4> bytes{};
    if (!read_bytes(bytes)) {return false;}
    n = std::to_integer<std::uint32_t>(bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3]);
    return true;
}

bool dns::WireReader::seek(std::size_t offset) {
    if (offset > buffer.size()) return false;
    pos = offset;   return true;
}

std::size_t dns::WireReader::offset() const {
    return pos;
}

std::size_t dns::WireReader::remaining() const {
    return buffer.size() - pos;
}