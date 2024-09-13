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

struct Token : TokenPos {
  enum EToken { // token 类型
    Inv,        // 非法 token
    Space,      // 空格
    Comment,    // 注释
    Int,        // 整数
    Float,      // 浮点数
    Fixed,      // 定点数
    Integer,    // 高精度整数
    RealNum,    // 高精度实数
    Str,        // 字符串
    Chr,        // 字符
    Op,         // 运算符
    Attr,       // 属性
    Punc,       // 标点符号
    Sym,        // 标识符
    Kwd,        // 关键字
    Cnt,        // 最大值
    Num,        // 数字通配
  };

  EToken type;
  str    raw; // token 对应的字符串

  Token(EToken type, const str &raw, const TokenPos &pos);
  virtual ~Token() = default;

  auto copy() const -> Token * {
    return new Token(type, raw, *this);
  }

  auto operator==(const str &raw) const -> bool {
    return this->raw == raw;
  }

  friend auto operator<<(ostream &os, Token::EToken t) -> ostream &;
  friend auto operator<<(ostream &os, const Token &tok) -> ostream &;
  friend auto operator<<(ostream &os, const Token *tok) -> ostream &;

  auto is(EToken type) const -> bool;
};

// --------------------------------------------------

struct Inv : Token {
  EToken matchtype; // 匹配到什么类型的 token 但解析出错时创建的 Inv
  str    msg;       // 错误信息

  Inv(const str &raw, const TokenPos &pos, EToken matchtype = Token::Inv, const str &msg = "");
  ~Inv() override = default;
};

// --------------------------------------------------

struct Space : Token {
  int indent;

  Space(const str &raw, const TokenPos &pos);
  ~Space() override = default;
};

struct Comment : Token {
  bool is_line; // 是否是行注释
  str  text;    // 注释内容

  Comment(const str &raw, const TokenPos &pos);
  ~Comment() override = default;
};

// --------------------------------------------------

struct Int : Token {
  int  base;        // 字面量使用的进制
  int  nbits;       // 位数
  bool is_unsigned; // 是否是无符号数
  int  is_imag;     // 是否是虚数 0不是, 1i, 2j, 3k
  mpz  val;         // 值

  Int(str raw, const TokenPos &pos);
  ~Int() override = default;
};

struct Float : Token {
  int nbits; // 位数
  mpf val;   // 值

  Float(str raw, const TokenPos &pos);
  ~Float() override = default;
};

struct Chr : Token {
  bytes val;

  Chr(str s, const TokenPos &pos) : Token(Token::Chr, s, pos) {}
  ~Chr() override = default;
};

struct Str : Token {
  bytes val;

  Str(str s, const TokenPos &pos) : Token(Token::Str, s, pos) {}
  ~Str() override = default;
};

// --------------------------------------------------

// 用户输入错误
class Error : public ::Error {
public:
  explicit Error(const str &msg) : ::Error(msg) {}
};

// 程序内部错误
class Fail : public ::Error {
public:
  explicit Fail(const str &msg) : ::Error(msg) {}
};

// --------------------------------------------------

auto mktoken(Token::EToken type, str raw, TokenPos pos) -> Token *;

auto num_type_suffix(str raw, bool is_float) -> Tuple<Token::EToken, int, int>;

} // namespace lumos::token

namespace lumos {
using token::Token;
}
