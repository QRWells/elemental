#include "runtime/runtime.h"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <span>
#include <vector>

#include "elements/atom.hpp"
#include "elements/membrane.hpp"
#include "elements/rule.hpp"
#include "il/format.hpp"

namespace {
template <std::unsigned_integral T> constexpr inline auto Fetch(std::span<std::byte>::iterator &pc) -> T {
  auto value = *reinterpret_cast<T *>(&*pc);
  pc += sizeof(T);
  return value;
}

} // namespace

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

void Runtime::Run() {
  auto *root = new elements::Membrane{"root"};
  this->membranesByRegister_.emplace(0, root);

  Execute(this->code_); // initialize environment
  // TODO: Try to find applicable rules. (Atom-based way)
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

  auto string_table = bytes.subspan(header.string_table_offset, header.string_table_length);
  auto code         = bytes.subspan(header.predef_offset, header.predef_length);
  auto rule         = bytes.subspan(header.rule_offset, header.rule_length);

  // TODO: Parse the string table and code sections.

  return TryLoadStringTable(string_table) && TryLoadCode(code) && TryLoadRule(rule);
}

auto Runtime::TryLoadStringTable(std::span<std::byte> bytes) -> bool {
  auto pc = bytes.begin();
  while (pc != bytes.end()) {
    auto length = Fetch<std::uint8_t>(pc);
    auto str    = std::string{reinterpret_cast<char *>(&*pc), length};
    pc += length;
    this->string_storage_.Insert(str);
  }
  return true;
}

auto Runtime::TryLoadCode(std::span<std::byte> bytes) -> bool {
  this->code_ = std::vector<std::byte>(bytes.begin(), bytes.end());
  return true;
}

auto Runtime::TryLoadRule(std::span<std::byte> bytes) -> bool {
  auto base            = bytes.begin();
  auto header          = bytes.begin();
  auto number_of_rules = Fetch<std::uint32_t>(header);
  for (auto i = 0; i < number_of_rules; ++i) {
    auto rule_base   = base + Fetch<std::uint32_t>(header);
    auto rule_header = il::RuleHeader::Parse({rule_base, bytes.end()});

    auto *mem = membranesByRegister_.at(rule_header.mem_id);
    // TODO: Parse the rule.
    auto rb   = elements::RuleBuilder{mem};
    auto rule = rb.Build();
    mem->AddRule(&rule);
  }
  return true;
}

void Runtime::Execute(std::span<std::byte> code) {
  auto pc = code.begin();
  while (pc != code.end()) {
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