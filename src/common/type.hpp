#pragma once

#include <cstddef>

class Membrane;
class Atom;

enum class ElementType {
  Membrane,
  Atom,
};

using NameID  = std::size_t;
using Element = void *;