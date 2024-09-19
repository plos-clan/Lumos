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

  auto update(cstr s, size_t n) -> TokenPos {
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

using TokenPosRef = const TokenPos &;

pstruct(Token), TokenPos {
  enum EToken { // token 类型
    Inv,        // 非法 token
    Space,      // 空格
    Comment,    // 注释
    Int,        // 整数
    Float,      // 浮点数
    Fixed,      // 定点数
    MPZ,        // 高精度整数
    MPQ,        // 高精度分数
    Str,        // 字符串
    FmtStrBeg,  // 格式化字符串开始 '"'
    FmtStrData, // 格式化字符串数据
    FmtStrEnd,  // 格式化字符串结束 '"'
    Op,         // 运算符
    Attr,       // 属性             '@xxx'
    RootNS,     // 根命名空间       '::'
    BlkBeg,     // 括号开始         '(' '[' '{'
    BlkEnd,     // 括号结束         ')' ']' '}'
    Punc,       // 分隔符           ',' ';' '.' '...'
    Sym,        // 标识符           (变量名)
    Kwd,        // 关键字
    Eof,        // 文件结束
    Cnt = Eof,  // 最大值
  };

  EToken type;   // token 类型
  str    raw;    // token 对应的字符串
  str    strval; // 字符串值

  Token(EToken type, strref raw, TokenPosRef pos);
  virtual ~Token() = default;

  auto copy() const->Token * {
    return new Token(type, raw, *this);
  }

  auto operator==(strref raw) const->bool {
    return this->raw == raw;
  }

  void print_to(ostream & os) const;

  friend auto operator<<(ostream &os, Token::EToken t)->ostream &;
  friend auto operator<<(ostream &os, const Token &tok)->ostream &;
  friend auto operator<<(ostream &os, const Token *tok)->ostream &;

  auto is(EToken type) const->bool;
};

// --------------------------------------------------

auto rawstring(str raw, TokenPosRef pos) -> Token *;

struct Inv : Token {
  EToken matchtype; // 匹配到什么类型的 token 但解析出错时创建的 Inv
  str    msg;       // 错误信息

  Inv(strref raw, TokenPosRef pos, EToken matchtype = Token::Inv, strref msg = "");
  ~Inv() override = default;
};

// --------------------------------------------------

struct Space : Token {
  int indent;

  Space(strref raw, TokenPosRef pos);
  ~Space() override = default;
};

struct Comment : Token {
  bool is_line; // 是否是行注释
  str  text;    // 注释内容

  Comment(strref raw, TokenPosRef pos);
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

  num_data(strref raw) {
    TokenPos pos;
    parse(pos, raw);
  }
  num_data(TokenPosRef pos, strref raw) {
    parse(pos, raw);
  }

  void parse(TokenPosRef pos, str raw);
  void println() const;
};

struct Num : Token, num_data {
  Num(EToken type, strref raw, TokenPosRef pos);
  ~Num() override = default;
};

struct Int : Num {
  mpz value; // 值
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

  Int(strref raw, TokenPosRef pos);
  ~Int() override = default;
};

struct Float : Num {
  mpf value; // 值
  f64 val64;

  Float(strref raw, TokenPosRef pos);
  ~Float() override = default;
};

struct MPZ : Num {
  mpz value; // 值

  MPZ(strref raw, TokenPosRef pos);
  ~MPZ() override = default;
};

struct MPQ : Num {
  mpq value; // 值

  MPQ(strref raw, TokenPosRef pos);
  ~MPQ() override = default;
};

// --------------------------------------------------

struct Str : Token {
  bytes value;

  Str(str s, TokenPosRef pos) : Token(Token::Str, s, pos) {}
  ~Str() override = default;
};

struct BlkBeg : Token {
  BlkBeg(strref raw, TokenPosRef pos) : Token(Token::BlkBeg, raw, pos) {}
};

struct BlkEnd : Token {
  BlkEnd(strref raw, TokenPosRef pos) : Token(Token::BlkEnd, raw, pos) {}
};

struct Sym : Token {
  Sym(strref raw, TokenPosRef pos) : Token(Token::Sym, raw, pos) {}
};

struct Kwd : Token {
  Kwd(strref raw, TokenPosRef pos) : Token(Token::Kwd, raw, pos) {}
};

// --------------------------------------------------

auto mktoken(Token::EToken type, strref raw, TokenPos pos) -> Token *;

} // namespace lumos::token

namespace lumos {
pusing(token, Token);
}
