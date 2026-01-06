#pragma once
#include "../ast.hpp"
#include "../container.hpp"
#include "type.hpp"

namespace lumos::ast {

class Struct : public Type, public Container {
  struct MemberVar {
    str      name;
    PType    type;
    PLiteral value;
  };
  struct MemberFunc {
    str       name;
    PFunction func;
  };

  Vector<MemberVar>  variables;
  Vector<MemberFunc> functions;

public:
  Struct()           = default;
  ~Struct() override = default;

  auto append(Named *child) -> void override;
};

// 结构体或类
class Class : public Type, public Container {
  struct MemberVar {
    str      name;
    PType    type;
    PLiteral value;
  };
  struct MemberFunc {
    str       name;
    PFunction func;
  };

  Vector<MemberVar>  variables;
  Vector<MemberFunc> functions;

public:
  Class()           = default;
  ~Class() override = default;

  auto append(Named *child) -> void override;
};

} // namespace lumos::ast
