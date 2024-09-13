#pragma once

#include <type.hpp>

namespace lumos::lexer {

enum {
  op_perfix,
  op_infix,
  op_suffix,
};

extern HashSet<str> ops_list;

extern HashSet<str> puncs_set;

} // namespace lumos::lexer