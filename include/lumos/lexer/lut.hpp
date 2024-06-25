#pragma once

#include <type.hpp>

namespace lumos::lexer {

// space -> 0
// sym -> 1
// op -> 2
// punc -> 3
// other-> 4

enum {
  lut_space,
  lut_sym,
  lut_op,
  lut_punc,
  lut_other,
};

extern u8 lut[256];

} // namespace lumos::lexer