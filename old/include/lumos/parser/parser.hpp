#pragma once

#include <lumos/ast.hpp>
#include <lumos/base.hpp>
#include <lumos/lexer.hpp>

namespace lumos::parser {

__ERRORIMPL__("Parser");

// --------------------------------------------------

// 上下文有关的语法解析器
class Parser {
  lexer::Lexer &lex;

  auto lexpeek() -> Token * {
    return lex.peek();
  }
  auto lexpeek(Token::EToken type) -> Token * {
    return lex.peek(type);
  }
  auto lexpeek(strref value) -> Token * {
    return lex.peek(value);
  }
  auto lexpeek(Token::EToken type, strref value) -> Token * {
    return lex.peek(type, value);
  }
  auto lexget() -> PToken {
    return lex.get();
  }
  auto lexget(Token::EToken type) -> PToken {
    return lex.get(type);
  }
  auto lexget(strref value) -> PToken {
    return lex.get(value);
  }
  auto lexget(Token::EToken type, strref value) -> PToken {
    return lex.get(type, value);
  }

public:
  ast::Container *container;

  auto find(strref name) const -> AST * {
    if (container == null) return null;
    return container->find(name);
  }

  explicit Parser(lexer::Lexer &lex) : lex(lex) {
    new ast::Root();
  }

  auto parse_fmtstr(); // 最阴间的玩意，格式化字符串

  auto parse_block() -> AST *; // 解析 using
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
