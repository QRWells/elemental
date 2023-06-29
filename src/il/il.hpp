#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>

namespace elemental::il {

constexpr std::size_t IlSize = 16;

enum class Op : uint8_t {
  // Atom operations
  AddAtom,
  RemoveAtom,
  MoveAtom,

  // Link operations
  Link,
  Unlink,

  // Membrane operations
  AddMembrane,
  RemoveMembrane,

  // Rule operations
  FindAtom,
  FindMembrane,

  // Type operations
  IsSame,
  IsUnary,
  IsInt,
  IsFloat,
  IsGround,
  IsHyperLink,

  // Expanded Instruction

  /// Indicates that the instruction is continued in the next instruction.
  Expanded,
};
} // namespace elemental::il