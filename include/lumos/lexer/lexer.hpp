#pragma once

#include "token.hpp"
#include <type.hpp>

namespace lumos::lexer {

__Pclass__(Lexer);

class Lexer : RefCount {
private:
  cstr   code = null; // 指向输入，随着解析向后移动
  size_t len  = 0;    // 储存总字符数
  size_t rem  = 0;    // 储存当前剩余字符数
  size_t pos  = 0;    // 储存当前 code 指向的是第几个字符
  u32    line = 1;    // 当前行数
  u32    col  = 1;    // 当前列数
  PToken _tok = null;

  // 输出一个 token
  auto token(Token::EToken type, size_t n) -> Token *;

  // 见 lexer/try-parse.cpp
  auto try_space() -> Token *;   // 尝试解析连续的空格
  auto try_comment() -> Token *; // 尝试解析注释
  auto try_macro() -> Token *;   // 尝试解析宏
  auto try_num() -> Token *;     // 尝试解析数字
  auto try_str() -> Token *;     // 尝试解析
  auto try_chr() -> Token *;     // 尝试解析
  auto try_op() -> Token *;      // 尝试解析
  auto try_attr() -> Token *;    // 尝试解析
  auto try_punc() -> Token *;    // 尝试解析
  auto try_sym() -> Token *;     // 尝试解析

  // 获取一个token
  auto _get() -> Token *;
  auto _get(const HashMap<str, Vector<str>> &priority) -> Token *;

public:
  bool return_space   = false; // 是否输出空格和注释
  bool return_invalid = false; // 是否输出非法字符
  bool log_tokens     = false; // 是否将 token 输出到日志

  explicit Lexer(const str &code);
  explicit Lexer(cstr code);
  Lexer(cstr code, size_t len);
  ~Lexer();

  auto eof() const -> bool; // 是否已解析完毕

  auto peek_char() const -> byte; // 读取一个字符 (不移动指针)
  auto get_char() -> byte;        // 读取一个字符 (移动指针)

  auto peek() -> PToken;    // 读取一个 token (不移动指针)
  auto get() -> PToken;     // 读取一个 token (移动指针)
  auto get_block() -> cstr; // 读取一个块
};

} // namespace lumos::lexer
