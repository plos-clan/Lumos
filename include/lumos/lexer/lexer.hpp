#pragma once

#include "token.hpp"
#include <lumos/base.hpp>

namespace lumos::lexer {

// 用户输入错误
class Error : public ::Error {
public:
  explicit Error(const str &msg) : ::Error("Lexer: " + msg) {}
};

// 程序内部错误
class Fail : public ::Error {
public:
  explicit Fail(const str &msg) : ::Error("[fail] Lexer: " + msg) {}
};

// --------------------------------------------------

using token::TokenPos;

// 保存解析状态
struct LexerState : TokenPos {
  cstr   code = null; // 指向输入，随着解析向后移动
  size_t len  = 0;    // 储存总字符数
  size_t rem  = 0;    // 储存当前剩余字符数
  Token *_tok = null;

  LexerState() = default;
  LexerState(const LexerState &stat);
  ~LexerState();
  auto operator=(const LexerState &stat) -> LexerState &;

  auto save() -> LexerState;                     // 保存解析状态
  void load(const LexerState &state);            // 加载解析状态
  auto update(size_t n) -> Pair<cstr, TokenPos>; // 更新指针位置 更新行数和列数
};

class Lexer : private LexerState {
  auto token(Token::EToken type, size_t n) -> Token *; // 输出一个 token
  auto token(Token::EToken type, size_t n, const str &raw) -> Token *;

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
  Ctx &ctx;                    // 全局上下文
  bool return_space   = false; // 是否输出空格和注释
  bool return_invalid = false; // 是否输出非法字符
  bool log_tokens     = false; // 是否将 token 输出到日志

  Vector<Pair<Token::EToken, str>> tryed; // 储存所有 token

  Lexer(Ctx &ctx, cstr code, size_t len);
  Lexer(Ctx &ctx, cstr file, cstr code, size_t len);
  ~Lexer() = default;

  using LexerState::load;
  using LexerState::save;

  auto eof() const -> bool;                                 // 是否已解析完毕
  auto peek_char() const -> byte;                           // 读取一个字符 (不移动指针)
  auto get_char() -> byte;                                  // 读取一个字符 (移动指针)
  auto peek() -> Token *;                                   // 读取一个 token (不移动指针)
  auto peek(Token::EToken type) -> Token *;                 // 读取一个 token (不移动指针)
  auto peek(const str &val) -> Token *;                     // 读取一个 token (不移动指针)
  auto peek(Token::EToken type, const str &val) -> Token *; // 读取一个 token (不移动指针)
  auto get() -> Token *;                                    // 读取一个 token (移动指针)
  auto get(Token::EToken type) -> Token *;                  // 读取一个 token (移动指针)
  auto get(const str &val) -> Token *;                      // 读取一个 token (移动指针)
  auto get(Token::EToken type, const str &val) -> Token *;  // 读取一个 token (移动指针)
  auto get_block() -> cstr;                                 // 读取一个块
  auto getall() -> Vector<Token>;                           //
  void error(str msg = "");                                 // 抛出错误
};

} // namespace lumos::lexer

namespace lumos {
using lexer::Lexer;
}
