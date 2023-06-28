#pragma once

#include <cstdint>

namespace elemental::il {

enum class Op : uint8_t {
  // Atom operations
  AddAtom,
  RemoveAtom,

  // Link operations
  Link,
  Unlink,

  // Membrane operations
  AddMembrane,
  RemoveMembrane,

  // Rule operations
  FindAtom,
  AnyMembrane,

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

constexpr uint64_t OpMask = 0xFF;

struct IL final {
public:
  constexpr IL(uint64_t data) noexcept : data_{data} {}

  [[nodiscard]] constexpr auto GetOp() const noexcept -> Op { return static_cast<Op>(data_ & OpMask); }
  [[nodiscard]] constexpr auto GetRawData() const noexcept -> uint64_t { return data_; }

private:
  uint64_t data_;
};
} // namespace elemental::il