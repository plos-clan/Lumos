#pragma once

#include "token.hpp"
#include <type.hpp>

namespace lumos::lexer {

class Error : public ::Error {
public:
  explicit Error(const str &msg) : std::runtime_error("Lexer: " + msg) {}
};

__Pclass__(Lexer);

using token::TokenPos;

struct LexerState : TokenPos {
  cstr   code = null; // 指向输入，随着解析向后移动
  size_t len  = 0;    // 储存总字符数
  size_t rem  = 0;    // 储存当前剩余字符数
  Token *_tok = null;

  LexerState() = default;
  LexerState(const LexerState &stat);
  ~LexerState();
  auto operator=(const LexerState &stat) -> LexerState &;

  auto save() -> LexerState;
  void load(const LexerState &state);
  auto update(size_t n) -> Pair<cstr, TokenPos>; // 更新指针位置 更新行数和列数
};

class Lexer : private LexerState {
  auto token(Token::EToken type, size_t n) -> Token *; // 输出一个 token

  auto try_space() -> Token *;   // 尝试解析连续的空字符
  auto try_comment() -> Token *; // 尝试解析注释
  auto try_macro() -> Token *;   // 尝试解析宏
  auto try_num() -> Token *;     // 尝试解析数字
  auto try_str() -> Token *;     // 尝试解析
  auto try_chr() -> Token *;     // 尝试解析
  auto try_op() -> Token *;      // 尝试解析
  auto try_attr() -> Token *;    // 尝试解析
  auto try_punc() -> Token *;    // 尝试解析
  auto try_sym() -> Token *;     // 尝试解析

  auto _get() -> Token *; // 获取一个token

public:
  bool return_space   = false; // 是否输出空格和注释
  bool return_invalid = false; // 是否输出非法字符
  bool log_tokens     = false; // 是否将 token 输出到日志

  Lexer(cstr file, cstr code, size_t len);
  ~Lexer();

  using LexerState::load;
  using LexerState::save;

  auto eof() const -> bool;       // 是否已解析完毕
  auto peek_char() const -> byte; // 读取一个字符 (不移动指针)
  auto get_char() -> byte;        // 读取一个字符 (移动指针)
  auto peek() -> Token *;         // 读取一个 token (不移动指针)
  auto get() -> Token *;          // 读取一个 token (移动指针)
  auto get_block() -> cstr;       // 读取一个块
};

} // namespace lumos::lexer
