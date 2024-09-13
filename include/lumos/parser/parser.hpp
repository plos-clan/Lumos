#pragma once

#include <lumos/ast.hpp>
#include <lumos/base.hpp>
#include <lumos/lexer.hpp>

namespace lumos::parser {

// 用户输入错误
class Error : public ::Error {
public:
  explicit Error(const str &msg) : ::Error("Parser: " + msg) {}
};

// 程序内部错误
class Fail : public ::Error {
public:
  explicit Fail(const str &msg) : ::Error("[fail] Parser: " + msg) {}
};

// --------------------------------------------------

void try_func_impl();

auto parse_using() -> AST *;

auto parse(Lexer &lex) -> AST *;

} // namespace lumos::parser
