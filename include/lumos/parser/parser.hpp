#pragma once

#include <lumos/ast.hpp>
#include <lumos/base.hpp>
#include <lumos/lexer.hpp>

namespace lumos::parser {

__ERRORIMPL__("Parser");

// --------------------------------------------------

class Parser {
  lexer::Lexer &lex;

public:
  explicit Parser(lexer::Lexer &lex) : lex(lex) {}

  auto parse_fmtstr(); // 最阴间的玩意，格式化字符串

  auto parse_using() -> AST *; // 解析 using
  void try_func_impl();
  auto parse_function() -> ast::Function *;
  auto try_function(bool allow_lambda = true) -> ast::Function *;
  auto parse_namespace(strref name) -> ast::Namespace *;
  auto try_namespace() -> ast::Namespace *;
  auto parse() -> AST *; // 语法解析开始
};

} // namespace lumos::parser

namespace lumos {
using parser::Parser;
}
