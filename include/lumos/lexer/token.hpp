#pragma once

#include <lumos/base.hpp>

namespace lumos::token {

__ERRORIMPL__("Token");

// --------------------------------------------------

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

pstruct(Token), TokenPos {
  enum EToken { // token 类型
    Inv,        // 非法 token
    Space,      // 空格
    Comment,    // 注释
    Int,        // 整数
    Float,      // 浮点数
    Fixed,      // 定点数
    Integer,    // 高精度整数
    Frac,       // 高精度分数
    Str,        // 字符串
    Chr,        // 字符
    Op,         // 运算符
    Attr,       // 属性
    Punc,       // 标点符号
    Sym,        // 标识符
    Kwd,        // 关键字
    Cnt,        // 最大值
  };

  EToken type; // token 类型
  str    raw;  // token 对应的字符串

  Token(EToken type, const str &raw, const TokenPos &pos);
  virtual ~Token() = default;

  auto copy() const->Token * {
    return new Token(type, raw, *this);
  }

  auto operator==(const str &raw) const->bool {
    return this->raw == raw;
  }

  virtual void print_to(ostream & os) const;

  friend auto operator<<(ostream &os, Token::EToken t)->ostream &;
  friend auto operator<<(ostream &os, const Token &tok)->ostream &;
  friend auto operator<<(ostream &os, const Token *tok)->ostream &;

  auto is(EToken type) const->bool;
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

auto num_type_suffix(str raw, bool is_float) -> Tuple<Token::EToken, int, int>;

struct num_data {
  mpz  int_part;            // 整数部分
  mpz  dec_part;            // 小数部分
  mpz  dec_deno;            // 小数部分的分母
  mpz  exp_part;            // 指数部分
  int  is_imag     = 0;     // 是否是虚数 0不是, 1i, 2j, 3k
  bool is_unsigned = false; // 是否是无符号数
  int  nbits       = 0;     // 位数

  num_data(str raw);

  void println() const;
};

struct Num : Token, num_data {
  Num(EToken type, const str &raw, const TokenPos &pos);
  ~Num() override = default;
};

struct Int : Num {
  mpz val; // 值
  union {
    i8  val8;
    u8  valu8;
    i16 val16;
    u16 valu16;
    i32 val32;
    u32 valu32;
    i64 val64;
    u64 valu64;
  };

  Int(const str &raw, const TokenPos &pos);
  ~Int() override = default;
};

struct Float : Num {
  mpf val; // 值
  f64 val64;

  Float(const str &raw, const TokenPos &pos);
  ~Float() override = default;
};

struct Integer : Num {
  mpz val; // 值

  Integer(const str &raw, const TokenPos &pos);
  ~Integer() override = default;
};

struct Frac : Num {
  mpq val; // 值

  Frac(const str &raw, const TokenPos &pos);
  ~Frac() override = default;
};

// --------------------------------------------------

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

auto mktoken(Token::EToken type, str raw, TokenPos pos) -> Token *;

} // namespace lumos::token

namespace lumos {
pusing(token, Token);
}
