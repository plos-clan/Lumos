#pragma once

#include "type.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Type
//* ----------------------------------------------------------------------------------------------------

class PtrType : public Type {
public:
  size_t level = 0; // 0 引用, > 0 指针

  explicit PtrType(Type *type);
  PtrType(Type *type, size_t level);

  auto tostr() const -> str override;

  auto check() const -> void override;
};

class ArrayType : public Type {
public:
  PType  type;
  size_t len = 0;

  explicit ArrayType(Type *type) : type(type) {}
  ArrayType(Type *type, size_t len) : type(type), len(len) {
    if (len == 0) throw Error("不能定义一个长度为0的数组");
  }

  auto tostr() const -> str override;

  auto check() const -> void override;
};

// 引用计数的类型
class RefCntType : public Type {
public:
  PType type;

  explicit RefCntType(Type *type);

  auto tostr() const -> str override;

  auto check() const -> void override;
};

} // namespace lumos::ast
