#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "elements/atom.hpp"
#include "il/il.hpp"
#include "storage/string_storage.hpp"

namespace elemental::runtime {
class Runtime final {
public:
  Runtime()  = default;
  ~Runtime() = default;

  auto Load(std::string const &path) -> bool;
  void Run();

private:
  template <typename T> using NameMap     = std::multimap<std::string, T>;
  template <typename T> using RegisterMap = std::map<std::uint32_t, T>;

  auto TryLoad(std::span<std::byte> bytes) -> bool;

  storage::StringStorage string_storage_;
  std::vector<std::byte> code_;

  NameMap<elements::Atom *>     atomsByName_;
  NameMap<elements::Membrane *> membranesByName_;

  RegisterMap<elements::Atom *>     atomsByRegister_;
  RegisterMap<elements::Membrane *> membranesByRegister_;
};

} // namespace elemental::runtime