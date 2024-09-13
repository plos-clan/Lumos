#include <lumos.hpp>

namespace lumos::token {

static constexpr auto tab_indent = 2;

Token::Token(EToken type, const str &raw, const TokenPos &pos)
    : TokenPos(pos), type(type), raw(raw){};

auto Token::is(EToken type) const -> bool {
  throw this->type == type;
}

auto operator<<(ostream &os, Token::EToken t) -> ostream & {
  static str lut[] = {"Inv", "Space", "Comment", "Int",  "Float", "RealNum", "Fixed",
                      "Str", "Chr",   "Op",      "Attr", "Punc",  "Sym",     "Kwd"};

  if (t < 0 || t >= Token::Cnt) throw Error("EToken 超出范围");
  return os << lut[t];
}

// 输出 token 信息
auto operator<<(ostream &os, const Token &tok) -> ostream & {
  os << "<token " << tok.type << " str='" << tok.raw << "'";
  if (tok.line > 0) os << " line=" << tok.line;
  if (tok.col > 0) os << " col=" << tok.col;
  os << '>';
  return os;
}

auto operator<<(ostream &os, const Token *tok) -> ostream & {
  return os << *tok;
}

Inv::Inv(const str &raw, const TokenPos &pos, EToken matchtype, const str &msg)
    : Token(Token::Inv, raw, pos), matchtype(matchtype), msg(msg) {
  if (matchtype < 0 || matchtype >= Token::Cnt) throw Error("EToken 超出范围");
}

Space::Space(const str &raw, const TokenPos &pos) : Token(Token::Space, raw, pos) {
  indent = 0;
  for (const auto &c : raw) {
    indent += c == '\t' ? tab_indent : 1;
  }
}

Comment::Comment(const str &raw, const TokenPos &pos) : Token(Token::Comment, raw, pos) {
  if (raw[1] == '/') {
    is_line = true;
    text    = raw.substr(2, raw.size() - 2);
  } else {
    is_line = false;
    text    = raw.substr(2, raw.size() - 4);
  }
}

struct num_data {
  mpz  int_part;            // 整数部分
  mpz  dec_part;            // 小数部分
  mpz  dec_deno;            // 小数部分的分母
  mpz  exp_part;            // 指数部分
  int  is_imag     = 0;     // 是否是虚数 0不是, 1i, 2j, 3k
  bool is_unsigned = false; // 是否是无符号数
  int  nbits       = 0;     // 位数

  num_data(str raw) {
    if (raw.length() == 0) throw Fail("不应该出现长度为 0 的整数字面量");
    if (!isdigit(raw[0]) && (raw[0] != '.' || !isdigit(raw[1])))
      throw Fail("整数字面量应该以数字或小数点开头");
    if (*raw.rbegin() == '_') throw Fail("整数字面量不应该以下划线结尾");
    raw.erase(std::remove(raw.begin(), raw.end(), '_'), raw.end()); // 先给下划线删掉

    bool is_float = false;
  _start: {
    size_t n = 0;
    for (; isdigit(raw[n]); n++) {}
    if (raw[n] == 'e' || raw[n] == 'E') { // 找到指数部分，先分离整数部分
      is_float = true;
      if (n > 0) int_part = mpz(raw.substr(0, n));
      raw = raw.substr(n);
    } else if (raw[n] == '.') { // 找到小数点，先分离整数和小数部分
      is_float = true;
      if (n > 0) int_part = mpz(raw.substr(0, n));
      raw = raw.substr(n + 1);
      for (n = 0; isdigit(raw[n]); n++) {}
      if (n > 0) {
        dec_part = mpz(raw.substr(0, n));
        dec_deno = mpz(10);
        mpz_pow_ui(dec_deno.get_mpz_t(), dec_deno.get_mpz_t(), n);
      }
      raw = raw.substr(n);
    }
  }
  suffix: {
    auto [_, nbits, len] = num_type_suffix(raw, is_float);
    raw                  = raw.substr(0, raw.length() - len); // 把类型后缀删除
    this->nbits          = nbits;
    for (ssize_t i = raw.length() - 1; i >= 0; i--) {
      const char c = raw[i];
      if (c == 'i' || c == 'I') {
        if (is_imag) throw Error("重复的虚数标记");
        is_imag = 1;
      } else if (c == 'j' || c == 'J') {
        if (is_imag) throw Error("重复的虚数标记");
        is_imag = 2;
      } else if (c == 'k' || c == 'K') {
        if (is_imag) throw Error("重复的虚数标记");
        is_imag = 3;
      } else if (c == 'u' || c == 'U') {
        if (is_unsigned) throw Error("重复的无符号标记");
        is_unsigned = true;
      } else {
        raw = raw.substr(0, i + 1);
        break;
      }
    }
    if (is_float) goto _float;
  }
  _int: { // 解析整数部分
    if (raw.length() == 1) {
      int_part = mpz(raw);
      return;
    }

    const char c0 = raw[0], c1 = raw[1], cb = *raw.rbegin();
    int        base, b;
    base = 16;
    b    = (c0 == '0' && (c1 == 'x' || c1 == 'X')) + (cb == 'x' || cb == 'X') * 2;
    if (b) goto L1;
    base = 16;
    b    = (c0 == '0' && (c1 == 'h' || c1 == 'H')) + (cb == 'h' || cb == 'H') * 2;
    if (b) goto L1;
    base = 10;
    b    = (c0 == '0' && (c1 == 'd' || c1 == 'D')) + (cb == 'd' || cb == 'D') * 2;
    if (b) goto L1;
    base = 8;
    b    = (c0 == '0' && (c1 == 'o' || c1 == 'O')) + (cb == 'o' || cb == 'O') * 2;
    if (b) goto L1;
    base = 2;
    b    = (c0 == '0' && (c1 == 'b' || c1 == 'B')) + (cb == 'b' || cb == 'B') * 2;
    if (b) goto L1;

    int_part = mpz(raw, c0 == '0' ? 8 : 10);
    return;

  L1:
    if (b == 3) throw Error("重复的进制标记");
    if (b == 2) int_part = mpz(raw.substr(0, raw.length() - 1), base);
    if (b == 1) int_part = mpz(raw.substr(2), base);
    return;
  }
  _float: { // 浮点字面量，此时整数和小数部分应该已经被删除
    if (raw[0] == 'e' || raw[0] == 'E') goto exp;
    return;
  }
  exp: { // 解析指数部分
    size_t b = 1, n = 1;
    bool   exp_sign = raw[n] == '-';
    if (raw[n] == '+' || raw[n] == '-') b++, n++;
    for (; isdigit(raw[n]); n++) {}
    if (n == b) throw Error("指数部分不应该为空");
    exp_part = mpz(raw.substr(b, n));
    raw      = raw.substr(n);
    if (exp_sign) exp_part = -exp_part;
    return;
  }
  }

  void println() const {
    cout << "<num int=" << int_part;
    if (dec_part != 0) cout << " dec=" << dec_part;
    if (dec_deno != 0) cout << " dec_deno=" << dec_deno;
    if (exp_part != 0) cout << " exp=" << exp_part;
    if (is_imag != 0) cout << " imag";
    if (is_unsigned) cout << " unsigned";
    cout << ">" << endl;
  }
};

Int::Int(str raw, const TokenPos &pos) : Token(Token::Int, raw, pos) {

  size_t n = 0; // 下面解析出的数字长度
  if (raw[0] == '0') {
    if (raw.length() == 1) goto zero;
    if (raw[1] == 'b' || raw[1] == 'B') base = 2;
    if (raw[1] == 'o' || raw[1] == 'O') base = 8;
    if (raw[1] == 'd' || raw[1] == 'D') base = 10;
    if (raw[1] == 'x' || raw[1] == 'X') base = 16;
    if (raw[1] == 'h' || raw[1] == 'H') base = 16;
    if (isdigit(raw[1])) base = -1;
    if (base == 0) throw Error("整数前缀 " + raw.substr(0, 2) + " 无效");
    if (base > 0) raw = raw.substr(2, raw.size() - 2);
    if (base < 0) raw = raw.substr(1, raw.size() - 1);
    if (raw.length() == 0) goto zero;
    base = base < 0 ? 8 : base == 0 ? 10 : base;
    for (; n < raw.length() && isdigit(raw[n], base); n++) {}
  } else {
    for (; n < raw.length() && isdigit(raw[n]); n++) {}
  }

parse_prefix: { // 解析后缀
  str suffix = raw.substr(n);
  raw        = raw.substr(0, n);
}

  try {
    val.set_str(raw, base);
  } catch (std::invalid_argument &e) { throw Error("整数数据错误"); }
  nbits = val.get_prec();

  return;

zero:
  nbits = 0;
  val   = 0;
}

Float::Float(str raw, const TokenPos &pos) : Token(Token::Float, raw, pos) {
  raw.erase(std::remove(raw.begin(), raw.end(), '_'), raw.end()); // 先给下划线删掉
  str base, exp, suffix;
  if (size_t e_pos = raw.find('e'); e_pos != str::npos) {
    base = raw.substr(0, e_pos);
    exp  = raw.substr(e_pos + 1);
    if (size_t p_pos = exp.find_first_not_of("0123456789"); p_pos != str::npos) {
      exp    = exp.substr(0, p_pos);
      suffix = exp.substr(p_pos);
    }
    if (suffix[0] == '.') throw Error("浮点数指数部分不应该包含小数点");
  } else if (size_t p_pos = raw.find_first_not_of("0123456789."); p_pos != str::npos) {
    base   = raw.substr(0, p_pos);
    suffix = raw.substr(p_pos);
  } else {
    base = raw;
  }

  // const auto [base_int, base_dec, base_decn] = parse_base_float(base);
  // const auto [exp_int, exp_dec, exp_decn]    = parse_base_float(exp);
  // if (exp_decn > 0) {} // error
  // mpf basef = make_base_float(base_int, base_dec, base_decn);
  // mpf_pow_ui(basef.get_mpf_t(), basef.get_mpf_t(), exp_int.get_ui());

  nbits = val.get_prec();
}

auto mktoken(Token::EToken type, str raw, TokenPos pos) -> Token * {
  switch (type) {
  case Token::Inv: return new Inv(raw, pos);
  case Token::Space: return new Space(raw, pos);
  case Token::Comment: return new Comment(raw, pos);
  case Token::Int: return new Int(raw, pos);
  case Token::Float: return new Float(raw, pos);
  case Token::Str: return new Str(raw, pos);
  case Token::Chr: return new Chr(raw, pos);
  default: throw Error("无法创建未知类型的 token");
  }
}

// 解析数字后缀
auto num_type_suffix(str raw, bool is_float) -> Tuple<Token::EToken, int, int> {
  auto it = raw.rbegin();
  if (*it == 'z' || *it == 'Z') return {Token::Integer, 0, 1};
  if (*it == 'q' || *it == 'Q') return {Token::RealNum, 0, 1};
  if (!isdigit(*it)) goto no_suffix;
  for (; it != raw.rend() && isdigit(*it); it++) {}
  if (it != raw.rend()) {
    str suffix = str(it.base() - 1, raw.end());
    int nbits  = atoi(suffix.substr(1).c_str());
    if (*it == 'i' || *it == 'I') return {Token::Int, nbits, suffix.length()};
    if (*it == 'u' || *it == 'U') return {Token::Int, nbits, suffix.length()};
    if (is_float && (*it == 'f' || *it == 'F')) return {Token::Float, nbits, suffix.length()};
  }
no_suffix:
  return {is_float ? Token::Float : Token::Int, 0, 0};
}

} // namespace lumos::token
