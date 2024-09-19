#include <lumos.hpp>

using namespace lumos::ast;

namespace lumos::parser {

auto Parser::parse_using() -> AST * {
  if (lex.get(Token::Str)) {
    if (lex.get(";")) {}
    logger.info("using: " + lex.get()->raw);
  }
}

auto Parser::parse_namespace(strref name) -> Namespace * {
  if (lex.get("{") == null) lex.error();
  auto ns = new Namespace(null, name);
  while (!lex.eof() && !lex.get("}")) {
    Named *ast = null;
    if (ast == null) ast = try_function(false);
    if (ast == null) lex.error("无法解析的语法");
    ns->append(ast);
  }
  if (lex.get("}") == null) lex.error();
  return ns;
}

// namespace xxx {
// }
auto Parser::try_namespace() -> Namespace * {
  if (!lex.get("namespace")) return null;
  auto name = lex.get(Token::Sym);
  if (name == null) lex.error("命名空间名不能为空");
  return parse_namespace(name->raw);
}

auto Parser::parse() -> AST * {
  try_namespace();
  if (lex.get("fn")) return null;
  if (lex.get("private")) return null;
  return null;
}

} // namespace lumos::parser
