#pragma once

#include "il/il.hpp"
#include "storage/string_storage.hpp"
#include <span>
#include <vector>

namespace elemental::runtime {
class Runtime final {
public:
  Runtime()  = default;
  ~Runtime() = default;

  auto Load(std::string const &path) -> bool;
  auto Run() -> bool;

  void InsertString(std::string const &string) { string_storage_.Insert(string); }

  auto TryLoad(std::span<std::byte> bytes) -> bool;

private:
  storage::StringStorage string_storage_;
  std::vector<il::IL>    code_;
};

} // namespace elemental::runtime