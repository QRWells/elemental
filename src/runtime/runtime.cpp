#include "runtime/runtime.h"

#include <concepts>
#include <cstddef>
#include <fstream>
#include <span>
#include <vector>

#include "elements/atom.hpp"
#include "elements/membrane.hpp"
#include "il/format.hpp"

namespace elemental::runtime {

auto Runtime::Load(std::string const &path) -> bool {
  auto file = std::fstream{path, std::ios::in | std::ios::binary | std::ios::ate};
  if (!file.is_open()) {
    return false;
  }
  auto size = file.tellg();
  file.seekg(0, std::ios::beg);
  auto bytes = std::vector<std::byte>(size);
  if (!file.read(reinterpret_cast<char *>(bytes.data()), size)) {
    return false;
  }

  return TryLoad(bytes);
}

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

  auto string_table = bytes.subspan(header.string_table_length, header.string_table_length);
  auto code         = bytes.subspan(header.code_offset, header.code_length);

  // TODO: Parse the string table and code sections.

  while (!string_table.empty()) {
    auto length = *reinterpret_cast<uint8_t *>(string_table.data());
    auto string = std::string{reinterpret_cast<char *>(string_table.data() + 1), length};
    string_storage_.Insert(string);

    string_table = string_table.subspan(length + 1);
  }

  code_.resize(code.size());
  std::memcpy(code_.data(), code.data(), code.size());

  return true;
}

template <std::unsigned_integral T> constexpr auto Fetch(std::vector<std::byte>::iterator &pc) -> T {
  auto value = *reinterpret_cast<T *>(&*pc);
  pc += sizeof(T);
  return value;
}

void Runtime::Run() {
  auto pc = this->code_.begin();
  while (pc != this->code_.end()) {
    auto op = static_cast<il::Op>(*pc);
    switch (op) {
    case il::Op::AddAtom: {
      auto reg_id  = Fetch<std::uint32_t>(pc);
      auto mem_id  = Fetch<std::uint32_t>(pc);
      auto name_id = Fetch<std::uint32_t>(pc);

      auto *mem  = this->membranesByRegister_.at(mem_id);
      auto  name = this->string_storage_.Get(name_id);
      auto *atom = new elements::Atom{mem, name};
      mem->AddAtom(atom);
      this->atomsByName_.emplace(name, atom);
      this->atomsByRegister_.emplace(reg_id, atom);

      break;
    }
    case il::Op::RemoveAtom:
    case il::Op::MoveAtom:
    case il::Op::Link:
    case il::Op::Unlink:
    case il::Op::AddMembrane:
    case il::Op::RemoveMembrane:
    case il::Op::FindAtom:
    case il::Op::FindMembrane:
    case il::Op::IsSame:
    case il::Op::IsUnary:
    case il::Op::IsInt:
    case il::Op::IsFloat:
    case il::Op::IsGround:
    case il::Op::IsHyperLink:
    case il::Op::Expanded:
      break;
    }
    ++pc;
  }
}

} // namespace elemental::runtime