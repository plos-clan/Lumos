#pragma once

#include <type.hpp>

namespace lumos::lexer {

__Pclass__(Token);

class Token : RefCount {
public:
  // 词元的类型
  enum EToken {
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

  EToken type    = Inv;
  EToken subtyoe = Inv;
  size_t pos     = 0;
  u32    line    = 0;
  u32    col     = 0;
  str    s; // token 对应的字符串
  str    v;

  Token(EToken type, str val) : type(type), subtyoe(type), s(std::move(val)) {}
  Token(EToken type, str val, size_t pos, u32 line, u32 col)
      : type(type), subtyoe(type), s(std::move(val)), pos(pos), line(line), col(col) {}

  auto operator==(const str &s) -> bool {
    return this->s == s;
  }

  friend auto operator<<(ostream &os, EToken t) -> ostream &;

  friend auto operator<<(ostream &os, const Token &tok) -> ostream &;
};

} // namespace lumos::lexer
