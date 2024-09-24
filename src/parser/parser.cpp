#include <lumos.hpp>

using namespace lumos::ast;

namespace lumos::parser {

auto Parser::parse_using() -> AST * {
  if (lex.get(Token::Str)) {
    if (lex.get(";")) {}
    logger.info("using: " + lex.get()->raw);
  }
}

static auto func_params() -> ast::AST * {
  return null;
}

auto Parser::try_function(bool allow_lambda) -> ast::Function * {
  if (!lex.get("fn")) return null;
  str name;
  if (auto tok = lex.get(Token::Sym); tok) name = tok->raw;

  if (lex.get("(") == null) lex.error();
  auto fn = new Function(null, name->raw);
  while (!lex.eof() && !lex.get(")")) {
    auto arg = lex.get(Token::Sym);
    if (arg == null) lex.error("参数名不能为空");
    fn->append(new Argument(null, arg->raw));
    if (lex.get(",") == null) break;
  }
  if (lex.get(")") == null) lex.error();
  if (lex.get("->")) {
    auto ret = lex.get(Token::Sym);
    if (ret == null) lex.error("返回值不能为空");
    fn->ret = new Type(null, ret->raw);
  }
  if (lex.get("{") == null) lex.error();
  while (!lex.eof() && !lex.get("}")) {
    auto stmt = lex.get(Token::Str);
    if (stmt == null) lex.error("语句不能为空");
    fn->append(new Statement(null, stmt->raw));
  }
  if (lex.get("}") == null) lex.error();
  return fn;
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
