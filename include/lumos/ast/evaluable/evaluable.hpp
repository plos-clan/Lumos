#pragma once

#include "../type.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Evaluable
//* ----------------------------------------------------------------------------------------------------

class Evaluable : public AST {
public:
  PType type; // 该节点求值的类型
};

} // namespace lumos::ast
