#include <lumos.hpp>

namespace lumos::parser {

auto parse(Lexer &lex) -> AST * {
  lex.get();
  return null;
}

} // namespace lumos::parser
