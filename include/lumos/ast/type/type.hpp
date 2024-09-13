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

  // void check() const override;
};

} // namespace lumos::ast
