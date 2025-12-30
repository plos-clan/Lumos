#pragma once

#include "type.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Type
//* ----------------------------------------------------------------------------------------------------

class PtrType : public Type {
public:
  Type *type;
  bool  nullable = false;

  explicit PtrType(Type *type, bool nullable = false) : type(type), nullable(nullable) {}

  auto tostr() const -> str override;

  // void check() const override;
};

class RefType : public Type {
public:
  Type *type;

  explicit RefType(Type *type) : type(type) {}

  auto tostr() const -> str override;

  // void check() const override;
};

class ArrayType : public Type {
public:
  Type  *type;
  size_t len = 0;

  explicit ArrayType(Type *type) : type(type) {}
  ArrayType(Type *type, size_t len) : type(type), len(len) {
    if (len == 0) throw Error("不能定义一个长度为0的数组");
  }

  auto tostr() const -> str override;

  // void check() const override;
};

// 引用计数的类型
class RefCntType : public Type {
public:
  Type *type;

  explicit RefCntType(Type *type);

  auto tostr() const -> str override;

  // void check() const override;
};

} // namespace lumos::ast
