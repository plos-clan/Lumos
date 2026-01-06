#pragma once

#include "../ast.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Type
//* ----------------------------------------------------------------------------------------------------

/**
 *\brief 类型（接口）
 *
 */
class Type : public AST {
public:
  Type()           = default;
  ~Type() override = default;

  virtual auto tostr() const -> str;

  void check() const override;
};

/**
 *\brief 具名类型，引用一个无名的类型
 *
 */
class NamedType : public Type, public Named {
  PType type;

public:
  NamedType(Container *parent, strref name, PType &type);
  ~NamedType() override = default;

  auto tostr() const -> str override;
};

} // namespace lumos::ast
