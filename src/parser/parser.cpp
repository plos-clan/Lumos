#include <lumos.hpp>

namespace lumos::parser {

auto parse_namespace(Lexer &lex, str name) -> AST * {}

auto try_namespace(Lexer &lex) -> AST * {
  if (!lex.get("namespace")) return null;
  auto name = lex.get(Token::Sym);
  if (name == null) lex.error();
  if (lex.get(";"))
    ;
  parse_namespace(lex, name->raw);
}

auto parse(Lexer &lex) -> AST * {
  if (lex.get("namespace")) {
    auto name = lex.get(Token::Sym);
    if (name == null) lex.error();
    if (lex.get(";"))
      ;
    parse_namespace(lex, name->raw);
  }
  if (lex.get("fn")) return null;
  if (lex.get("private")) return null;
  return null;
}

} // namespace lumos::parser
