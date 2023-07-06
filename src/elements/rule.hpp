#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "common/type.hpp"

namespace elemental::elements {

struct RuleItem {
public:
  RuleItem(ElementType type) : type_(type) {}

  [[nodiscard]] auto Type() const -> ElementType { return type_; }
  [[nodiscard]] auto Name() const -> std::optional<std::string> const & { return name_; }
  [[nodiscard]] auto Count() const -> std::size_t { return count_; }

  auto Name(std::string_view name) -> RuleItem & {
    name_ = std::string{name};
    return *this;
  }

  auto Count(std::size_t count) -> RuleItem & {
    count_ = count;
    return *this;
  }

private:
  /// The type of the rule item.
  ElementType type_;

  /// The name of the rule item.
  std::optional<std::string> name_{};

  /// The number of elements in the rule item.
  /// If the rule item is a membrane, then this is the number of elements in the
  /// membrane.
  /// If the rule item is an atom, then this is the number of links the atom
  /// has.
  std::size_t count_;
};

struct Rule {
public:
  [[nodiscard]] auto Empty() const -> bool { return items_.empty(); }

private:
  friend struct RuleBuilder;

  Rule(Membrane *mem) : membrane_{mem} {}

  Membrane              *membrane_;
  std::vector<std::byte> code_{};
  std::vector<RuleItem>  items_{};
};

inline auto FindAtom() -> RuleItem { return RuleItem{ElementType::Atom}; }
inline auto FindMembrane() -> RuleItem { return RuleItem{ElementType::Membrane}; }

struct RuleBuilder {
public:
  RuleBuilder(Membrane *mem) : rule_{mem} {}

  auto Build() -> Rule { return rule_; }
  auto Match(RuleItem const &item) -> RuleBuilder & {
    rule_.items_.emplace_back(item);
    return *this;
  }

private:
  Rule rule_;
};

} // namespace elemental::elements