#pragma once

#include "../ast.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Type
//* ----------------------------------------------------------------------------------------------------

class Type : public AST {
public:
  Type()           = default;
  ~Type() override = default;

  virtual auto tostr() const -> str;

  void check() const override;
};

class NamedType : public Type, public Named {
public:
  NamedType()           = default;
  ~NamedType() override = default;

  auto tostr() const -> str override;
};

} // namespace lumos::ast
