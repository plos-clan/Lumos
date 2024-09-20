#pragma once
#include "../ast.hpp"
#include "../container.hpp"
#include "type.hpp"

namespace lumos::ast {

class Struct : public Type, public Container {
  struct class_var {
    str      name;
    PType    type;
    PLiteral value;
  };
  struct class_func {
    str       name;
    PFunction func;
  };

  Vector<class_var>  variables;
  Vector<class_func> functions;

  Struct()           = default;
  ~Struct() override = default;

  auto append(Named *child) -> void override;
};

// 结构体或类
class Class : public Type, public Container {
  struct class_var {
    str      name;
    PType    type;
    PLiteral value;
  };
  struct class_func {
    str       name;
    PFunction func;
  };

  Vector<class_var>  variables;
  Vector<class_func> functions;

  Class()           = default;
  ~Class() override = default;

  auto append(Named *child) -> void override;
};

} // namespace lumos::ast
