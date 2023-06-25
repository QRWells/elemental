#pragma once

#include "common/type.hpp"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace elemental::elements {
struct Atom {
public:
  Atom(std::string_view name) : name_(name) {}

  [[nodiscard]] auto Name() const -> std::string_view { return name_; }
  [[nodiscard]] auto Arity() const -> std::size_t { return links_.size(); }

  auto AddLink(Atom *atom) -> void { links_.push_back(atom); }

private:
  Membrane           *membrane_;
  std::string         name_;
  std::vector<Atom *> links_;
};

} // namespace elemental::elements