#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "common/type.hpp"

using Hasher = std::function<NameID(std::string_view)>;

namespace elemental::storage {

class StringStorage {
public:
  StringStorage()                                          = default;
  StringStorage(StringStorage const &)                     = delete;
  StringStorage(StringStorage &&)                          = delete;
  auto operator=(StringStorage const &) -> StringStorage & = delete;
  auto operator=(StringStorage &&) -> StringStorage      & = delete;
  ~StringStorage()                                         = default;

  void SetHasher(Hasher hasher) { hasher_ = std::move(hasher); }

  auto Insert(std::string_view str) -> NameID {
    auto id = hasher_(str);
    id_to_str_.emplace(id, str);
    return id;
  }
  auto Get(NameID id) const -> std::string_view { return id_to_str_.at(id); }
  void Clear() { id_to_str_.clear(); }

  auto Size() const -> std::size_t { return id_to_str_.size(); }
  auto Empty() const -> bool { return id_to_str_.empty(); }
  void Reserve(std::size_t size) { id_to_str_.reserve(size); }

private:
  using StringMap = std::unordered_map<NameID, std::string>;

  Hasher    hasher_;
  StringMap id_to_str_;
};

} // namespace elemental::storage