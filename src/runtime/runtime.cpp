#include "runtime/runtime.h"

#include "il/format.hpp"

namespace elemental::runtime {

auto Runtime::TryLoad(std::span<std::byte> bytes) -> bool {
  using namespace ::elemental::il;
  auto header = Header::Parse(bytes);
  if (header.magic_number != MagicNumber) {
    return false;
  }

  // Only support version 1.0 of the file format.
  if (header.version != Version::Lmntal10) {
    return false;
  }

  // Check if the size of the code section is a multiple of 8.
  if (header.code_length % sizeof(uint64_t) != 0) {
    return false;
  }

  auto string_table = bytes.subspan(header.string_table_length, header.string_table_length);
  auto code         = bytes.subspan(header.code_offset, header.code_length);

  // TODO: Parse the string table and code sections.

  while (!string_table.empty()) {
    auto length = *reinterpret_cast<uint8_t *>(string_table.data());
    auto string = std::string{reinterpret_cast<char *>(string_table.data() + 1), length};
    string_storage_.Insert(string);

    string_table = string_table.subspan(length + 1);
  }

  while (!code.empty()) {
    auto instruction = *reinterpret_cast<uint64_t *>(code.data());
    code_.emplace_back(instruction);

    code = code.subspan(sizeof(uint64_t));
  }

  return true;
}

} // namespace elemental::runtime