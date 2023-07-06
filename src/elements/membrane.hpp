#pragma once

#include "common/type.hpp"
#include "elements/atom.hpp"
#include "elements/rule.hpp"
#include <cstddef>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_set>

namespace elemental::elements {

struct Membrane {
public:
  Membrane(std::string_view name = "") : name_(name) {}

  [[nodiscard]] auto Name() const -> std::string_view { return name_; }
  [[nodiscard]] auto Size() const -> std::size_t { return atoms_.size() + membranes_.size(); }
  [[nodiscard]] auto Empty() const -> bool { return atoms_.empty() && membranes_.empty(); }
  [[nodiscard]] auto AtomCount() const -> std::size_t { return atoms_.size(); }
  [[nodiscard]] auto MembraneCount() const -> std::size_t { return membranes_.size(); }

  auto FindAtom(std::string_view name, Membrane *parent, std::size_t arity) const -> std::optional<Atom *> {
    for (auto *atom : atoms_) {
      if (atom->GetName() == name && atom->GetMembrane() == parent && atom->GetArity() == arity) {
        return atom;
      }
    }
    return std::nullopt;
  }
  auto AddAtom(Atom *element) -> void { atoms_.emplace(element); }
  auto RemoveAtom(Atom *element) -> void { atoms_.erase(element); }

  auto AddMembrane(Membrane *element) -> void { membranes_.emplace(element); }
  auto RemoveMembrane(Membrane *element) -> void { membranes_.erase(element); }

  auto AddRule(Rule *rule) -> void { rules_.emplace(rule); }
  auto RemoveRule(Rule *rule) -> void { rules_.erase(rule); }

private:
  Membrane   *membrane_;
  std::string name_;

  std::unordered_set<Atom *>     atoms_;
  std::unordered_set<Membrane *> membranes_;
  std::unordered_set<Rule *>     rules_;
};

} // namespace elemental::elements
