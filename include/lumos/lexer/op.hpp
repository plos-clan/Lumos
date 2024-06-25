#pragma once

#include <type.hpp>

namespace lumos::lexer {

enum {
  op_perfix,
  op_infix,
  op_postfix,
};

extern HashSet<str> ops_list;

extern HashSet<str> puncs_list;

} // namespace lumos::lexer