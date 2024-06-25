#pragma once

#include "type.hpp"

namespace lumos::ast {

class InnerType : public Type {
public:
  InnerType()           = default;
  ~InnerType() override = default;

  auto tostr() const -> str override;

  auto check() const -> void override;
};

class IntType : public InnerType {
public:
  size_t nbits;

  explicit IntType(size_t nbits = 32) : nbits(nbits) {}
  ~IntType() override = default;

  auto tostr() const -> str override;

  auto check() const -> void override;
};

} // namespace lumos::ast
