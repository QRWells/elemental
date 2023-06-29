#pragma once

#include <cstddef>

namespace elemental::elements {
class Atom;
class Membrane;
} // namespace elemental::elements

enum class ElementType {
  Membrane,
  Atom,
};

using NameID  = std::size_t;