#include <lumos/lexer.hpp>

namespace lumos::lexer {

LexerState::LexerState(const LexerState &stat) {
  file = stat.file;
  pos  = stat.pos;
  line = stat.line;
  col  = stat.col;
  code = stat.code;
  len  = stat.len;
  rem  = stat.rem;
  if (stat._tok != null) _tok = stat._tok->copy();
}

LexerState::~LexerState() {
  if (_tok != null) delete _tok;
}

auto LexerState::operator=(const LexerState &stat) -> LexerState & {
  if (this == &stat) return *this;
  file = stat.file;
  pos  = stat.pos;
  line = stat.line;
  col  = stat.col;
  code = stat.code;
  len  = stat.len;
  rem  = stat.rem;
  if (_tok != null) delete _tok;
  if (stat._tok != null) _tok = stat._tok->copy();
  return *this;
}

auto LexerState::save() -> LexerState {
  return *this;
}

void LexerState::load(const LexerState &state) {
  *this = state;
}

auto LexerState::update(size_t n) -> Pair<cstr, TokenPos> {
  cstr s  = code;
  code   += n;
  rem    -= n;
  return {s, TokenPos::update(s, n)}; // 更新行数和列数
}

} // namespace lumos::lexer