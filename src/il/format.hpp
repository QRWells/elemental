#pragma once

#include <cstdint>
#include <cstring>
#include <span>

namespace elemental::il {
/// The magic number for the IL file format.
/// Represents the ASCII string "LMNB".
constexpr int32_t MagicNumber = 0x4C4D4E42;

enum class Version : uint8_t {
  Lmntal10 = 0x00,
};

enum FlagMask : uint8_t {
  HyperLinkMask = 0b10000000,
  ReservedMask  = 0b01111111,
};

struct Header {
  /// The magic number for the file.
  int32_t magic_number;
  /// Version of the file.
  Version version;
  /// Flags for the file.
  uint8_t flags;

  uint16_t reserved;

  /// The offset from the start of the file to the start of the string table.
  int32_t string_table_offset;
  /// The length of the string table in bytes.
  int32_t string_table_length;

  /// The offset from the start of the file to the start of the code section.
  int32_t code_offset;
  /// The length of the code section in bytes.
  int32_t code_length;

  static inline constexpr auto Parse(std::span<std::byte> bytes) noexcept {
    if (bytes.size() < sizeof(Header)) {
      return Header{.magic_number = -1}; // return invalid header
    }

    Header header;
    std::memcpy(&header, bytes.data(), sizeof(Header));
    return header;
  }
};
} // namespace elemental::il