#pragma once

#include "token.hpp"
#include <lumos/base.hpp>

namespace lumos::lexer {

__ERRORIMPL__("Lexer");

// --------------------------------------------------

using token::TokenPos;

// 保存解析状态
struct LexerState : TokenPos {
  cstr   code    = null;  // 指向输入，随着解析向后移动
  size_t len     = 0;     // 储存总字符数
  size_t rem     = 0;     // 储存当前剩余字符数
  size_t nfmtstr = 0;     // 在几层格式化字符串内
  bool   instr   = false; // 是否在字符串的纯文本部分内
  Token *_tok    = null;  //

  LexerState() = default;
  LexerState(const LexerState &stat);
  ~LexerState();
  auto operator=(const LexerState &stat) -> LexerState &;

  auto save() -> LexerState;                     // 保存解析状态
  void load(const LexerState &state);            // 加载解析状态
  auto update(size_t n) -> Pair<cstr, TokenPos>; // 更新指针位置 更新行数和列数
};

class Lexer : private LexerState {
  using EToken = Token::EToken;

  auto token(EToken type, size_t n) -> Token *; // 输出一个 token
  auto token(EToken type, size_t n, strref raw) -> Token *;

  auto try_space() -> Token *;   // 尝试解析连续的空字符
  auto try_comment() -> Token *; // 尝试解析注释
  auto try_macro() -> Token *;   // 尝试解析宏
  auto try_num() -> Token *;     // 尝试解析数字
  auto try_str() -> Token *;     // 尝试解析字符串
  auto try_chr() -> Token *;     // 尝试解析
  auto try_op() -> Token *;      // 尝试解析
  auto try_attr() -> Token *;    // 尝试解析
  auto try_rootns() -> Token *;  // 尝试解析根命名空间
  auto try_punc() -> Token *;    // 尝试解析分隔符
  auto try_sym() -> Token *;     // 尝试解析

  auto _get() -> Token *; // 获取一个token

public:
  CTX &ctx;                    // 全局上下文
  bool return_space   = false; // 是否输出空格和注释
  bool return_invalid = false; // 是否输出非法字符
  bool log_tokens     = false; // 是否将 token 输出到日志

  Vector<Pair<EToken, str>> tryed; // 储存所有 token

  Lexer(CTX &ctx, void *code, size_t len);
  Lexer(CTX &ctx, cstr file, void *code, size_t len);
  ~Lexer() = default;

  using LexerState::load;
  using LexerState::save;

  auto eof() const -> bool;                        // 是否已解析完毕
  auto peek_char() const -> byte;                  // 读取一个字符 (不移动指针)
  auto get_char() -> byte;                         // 读取一个字符 (移动指针)
  auto peek() -> Token *;                          // 读取一个 token (不移动指针)
  auto peek(EToken type) -> Token *;               // 读取一个 token (不移动指针)
  auto peek(strref value) -> Token *;              // 读取一个 token (不移动指针)
  auto peek(EToken type, strref value) -> Token *; // 读取一个 token (不移动指针)
  auto get() -> PToken;                            // 读取一个 token (移动指针)
  auto get(EToken type) -> PToken;                 // 读取一个 token (移动指针)
  auto get(strref value) -> PToken;                // 读取一个 token (移动指针)
  auto get(EToken type, strref value) -> PToken;   // 读取一个 token (移动指针)
  auto get_block() -> cstr;                        // 读取一个块
  auto getall() -> Vector<Token>;                  //
  void error(str msg = "");                        // 抛出错误
};

} // namespace lumos::lexer

namespace lumos {
using lexer::Lexer;
}
