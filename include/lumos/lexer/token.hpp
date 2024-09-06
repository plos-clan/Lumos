#pragma once

#include <type.hpp>

namespace lumos::token {

struct TokenPos {
  cstr   file = null; // 文件名
  size_t pos  = 0;    // 储存当前 code 指向的是第几个字符
  size_t line = 1;    // 当前行数
  size_t col  = 1;    // 当前列数

  auto update(str s, size_t n) -> TokenPos {
    TokenPos _pos  = *this;
    pos           += n;
    col           += n;
    for (size_t i = 0; i < n; i++) {
      if (s[i] == '\n') {
        line++;
        col = n - i;
      }
    }
    return _pos;
  }
};

class Token : public TokenPos {
public:
  enum EToken { // 词元的类型
    Inv,

    Space,
    Comment,

    Num,
    Int,
    Uint,
    Float,
    Fixed,

    Str,
    Chr,
    Op,
    Attr,
    Punc,

    Sym,
    Kwd,

    EToken_cnt,
  };

  EToken type; // 类型
  str    s;    // token 对应的字符串
  str    v;

  Token(EToken type, str val, TokenPos pos) : type(type), s(std::move(val)), TokenPos(pos) {}

  auto copy() const -> Token * {
    return new Token(type, s, *this);
  }

  auto operator==(const str &s) -> bool {
    return this->s == s;
  }

  friend auto operator<<(ostream &os, EToken t) -> ostream &;

  friend auto operator<<(ostream &os, const Token &tok) -> ostream &;
};

class Num {};

class Error : public ::Error {
public:
  explicit Error(const str &msg) : std::runtime_error(msg) {}
};

} // namespace lumos::token

namespace lumos {
using token::Token;
}
