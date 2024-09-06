#pragma once

#include <lumos/ast.hpp>
#include <lumos/lexer.hpp>
#include <type.hpp>

namespace lumos {
using ast::AST;
using lexer::Lexer;
} // namespace lumos

namespace lumos::parser {

__Pclass__(Parser);

class Parser : RefCount {
  void try_func_impl();
};

extern Map<str, AST *> modules_map;

auto parse_using() -> AST *;

auto parse(Lexer &lex) -> AST *;

} // namespace lumos::parser
