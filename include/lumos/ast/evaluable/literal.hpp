#pragma once

#include "evaluable.hpp"

namespace lumos::ast {

class Literal : public Evaluable {};

class Int : public Literal {};

} // namespace lumos::ast
