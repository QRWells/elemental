#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "common/type.hpp"

namespace elemental::elements {
struct Atom {
public:
  Atom(Membrane *mem, std::string_view name) : membrane_(mem), name_(name) {}

  [[nodiscard]] auto GetMembrane() const -> Membrane * { return membrane_; }
  [[nodiscard]] auto GetName() const -> std::string_view { return name_; }
  [[nodiscard]] auto GetArity() const -> std::size_t { return links_.size(); }

  auto AddLink(Atom *atom) -> void { links_.push_back(atom); }

private:
  Membrane           *membrane_;
  std::string         name_;
  std::vector<Atom *> links_;
};

} // namespace elemental::elements