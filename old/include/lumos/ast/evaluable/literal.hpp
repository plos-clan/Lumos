#pragma once

#include "evaluable.hpp"

namespace lumos::ast {

class Literal : public Expr {};

class Int : public Literal {};

} // namespace lumos::ast
