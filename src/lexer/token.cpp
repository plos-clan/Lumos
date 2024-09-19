#include <lumos.hpp>

namespace lumos::token {

static constexpr auto tab_indent = 4;

auto operator<<(ostream &os, Token::EToken t) -> ostream & {
  static const char *lut[] = {
      "Inv", "Space", "Comment", "Int",  "Float", "Fixed", "Integer", "Frac", "Str", "FSB",
      "FSD", "FSE",   "Op",      "Attr", "RNS",   "BB",    "BE",      "Punc", "Sym", "Kwd",
  };
  if (t < 0 || t >= Token::Cnt) throw Error("EToken 超出范围");
  return os << lut[t];
}

Token::Token(EToken type, strref raw, TokenPosRef pos) : TokenPos(pos), type(type), raw(raw) {}

auto Token::is(EToken type) const -> bool {
  throw this->type == type;
}

// 输出 token 信息
void Token::print_to(ostream &os) const {
  os << "<token " << type << " str='" << raw << "'";
  if (line > 0) os << " line=" << line;
  if (col > 0) os << " col=" << col;
  os << '>';
}

auto operator<<(ostream &os, const Token &tok) -> ostream & {
  tok.print_to(os);
  return os;
}

auto operator<<(ostream &os, const Token *tok) -> ostream & {
  tok->print_to(os);
  return os;
}

Space::Space(strref raw, TokenPosRef pos) : Token(Token::Space, raw, pos) {
  indent = 0;
  for (const auto &c : raw) {
    indent += c == '\t' ? tab_indent : 1;
  }
}

Comment::Comment(strref raw, TokenPosRef pos) : Token(Token::Comment, raw, pos) {
  if (raw[1] == '/') {
    is_line = true;
    text    = raw.substr(2, raw.size() - 2);
  } else {
    is_line = false;
    text    = raw.substr(2, raw.size() - 4);
  }
}

//* ----------------------------------------------------------------------------------------------------
//; 格式化字符串  字符串字面量
//* ----------------------------------------------------------------------------------------------------

auto rawstring(strref raw, TokenPosRef pos) -> Token * {
  if (raw.size() < 2) return null;
  Str *tok   = new Str(raw, pos);
  tok->value = raw.substr(3, raw.length() - 6);
  if (!raw.contains('\n')) return tok; // 单行的就直接返回原始字符串
  strbuilder sb;
  for (str line : raw.lines()) {
    ssize_t pos = line.find('|');
    if (pos < 0) {
      for (char c : line) {
        if (!isspace(c)) throw Error("多行字符串的每行必须以 `|` 开头");
      }
      continue;
    }
    sb += raw.substr(pos + 1);
  }
  tok->value = sb.str();
  return tok;
}

//* ----------------------------------------------------------------------------------------------------
//; 数字字面量
//* ----------------------------------------------------------------------------------------------------

// 解析数字后缀
auto num_type_suffix(str raw, bool is_float) -> Tuple<Token::EToken, int, int> {
  auto it = raw.rbegin();
  if (*it == 'z' || *it == 'Z') return {Token::MPZ, 0, 1};
  if (*it == 'q' || *it == 'Q') return {Token::MPQ, 0, 1};
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

void num_data::parse(TokenPosRef pos, str raw) {
  if (raw.length() == 0) throw Fail("不应该出现长度为 0 的整数字面量");
  if (!isdigit(raw[0]) && (raw[0] != '.' || !isdigit(raw[1])))
    throw Fail("整数字面量应该以数字或小数点开头");
  if (*raw.rbegin() == '_') throw Fail("整数字面量不应该以下划线结尾");
  raw = raw.remove('_'); // 先给下划线删掉

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
  ssize_t i            = raw.length() - 1;
  for (; i >= 0; i--) {
    const char c = raw[i];
    if (c == 'i' || c == 'I') {
      if (is_imag) {
        logger.error(pos, "重复的虚数标记");
        logger.fix("删除多于的虚数标记");
        throw Error("重复的虚数标记");
      }
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
  if (i < 0) raw = "";
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
  if (raw[0] == 'e' || raw[0] == 'E') { // 解析指数部分
    size_t b = 1, n = 1;
    bool   exp_sign = raw[n] == '-';
    if (raw[n] == '+' || raw[n] == '-') b++, n++;
    for (; isdigit(raw[n]); n++) {}
    if (n == b) throw Error("指数部分不应该为空");
    exp_part = mpz(raw.substr(b, n));
    raw      = raw.substr(n);
    if (exp_sign) exp_part = -exp_part;
  }
  if (raw.length() > 0) throw Error("浮点字面量解析后还剩下字符: " + raw);
  return;
}
}

void num_data::println() const {
  cout << "<num int=" << int_part;
  if (dec_part != 0) cout << " dec=" << dec_part;
  if (dec_deno != 0) cout << " dec_deno=" << dec_deno;
  if (exp_part != 0) cout << " exp=" << exp_part;
  if (is_imag != 0) cout << " imag";
  if (is_unsigned) cout << " unsigned";
  cout << ">" << endl;
}

// --------------------------------------------------
// 类

Num::Num(EToken type, strref raw, TokenPosRef pos) : Token(type, raw, pos), num_data(raw) {}

Int::Int(strref raw, TokenPosRef pos) : Num(Token::Int, raw, pos) {
  if (nbits != 0 && nbits != 8 && nbits != 16 && nbits != 32 && nbits != 64)
    throw Error("整数数位数只能是 8, 16, 32, 64");
  if (nbits == 0) nbits = value.fits_sint_p() ? 32 : 64;
  value = int_part;
  if (exp_part > 0) {
    mpz exp = 10;
    mpz_pow_ui(exp.get_mpz_t(), exp.get_mpz_t(), exp_part.get_si());
    value *= exp;
    if (dec_part != 0) value += dec_part * exp / dec_deno;
  }
  if (exp_part < 0) {
    mpz exp = 10;
    mpz_pow_ui(exp.get_mpz_t(), exp.get_mpz_t(), -exp_part.get_si());
    value /= exp;
  }
  auto v = value.get_si();
  if (nbits == 8) val8 = v;
  if (nbits == 16) val16 = v;
  if (nbits == 32) val32 = v;
  if (nbits == 64) val64 = v;
}

Float::Float(strref raw, TokenPosRef pos) : Num(Token::Float, raw, pos) {
  if (nbits != 0 && nbits != 32 && nbits != 64) throw Error("浮点数位数只能是 32 或 64");
  if (nbits == 0) nbits = 32;
  value = mpf(int_part);
  if (dec_part > 0) value += mpf(dec_part) / mpf(dec_deno);
  if (exp_part > 0) {
    mpf exp = 10;
    mpf_pow_ui(exp.get_mpf_t(), exp.get_mpf_t(), exp_part.get_si());
    value *= exp;
  }
  if (exp_part < 0) {
    mpf exp = 10;
    mpf_pow_ui(exp.get_mpf_t(), exp.get_mpf_t(), -exp_part.get_si());
    value /= exp;
  }
  val64 = value.get_d();
}

MPZ::MPZ(strref raw, TokenPosRef pos) : Num(Token::MPZ, raw, pos) {
  value = int_part;
  if (exp_part > 0) {
    mpz exp = 10;
    mpz_pow_ui(exp.get_mpz_t(), exp.get_mpz_t(), exp_part.get_si());
    value *= exp;
    if (dec_part != 0) value += dec_part * exp / dec_deno;
  }
  if (exp_part < 0) {
    mpz exp = 10;
    mpz_pow_ui(exp.get_mpz_t(), exp.get_mpz_t(), -exp_part.get_si());
    value /= exp;
  }
}

MPQ::MPQ(strref raw, TokenPosRef pos) : Num(Token::MPQ, raw, pos) {
  value = mpq(int_part);
  if (dec_part > 0) value += mpq(dec_part) / mpq(dec_deno);
  if (exp_part > 0) {
    mpz exp = 10;
    mpz_pow_ui(exp.get_mpz_t(), exp.get_mpz_t(), exp_part.get_si());
    value *= exp;
  }
  if (exp_part < 0) {
    mpz exp = 10;
    mpz_pow_ui(exp.get_mpz_t(), exp.get_mpz_t(), -exp_part.get_si());
    value /= exp;
  }
  value.canonicalize();
}

//* ----------------------------------------------------------------------------------------------------
//; 创建 token
//* ----------------------------------------------------------------------------------------------------

auto mktoken(Token::EToken type, strref raw, TokenPos pos) -> Token * {
#define caseof(_name_)                                                                             \
  case Token::_name_: return new _name_(raw, pos)
  switch (type) {
    caseof(Inv);
    caseof(Space);
    caseof(Comment);
    caseof(Int);
    caseof(Float);
    caseof(MPZ);
    caseof(MPQ);
    caseof(Str);
    caseof(Op);
    caseof(Attr);
    caseof(Punc);
    caseof(Sym);
    caseof(Kwd);
  default: throw Fail("无法创建未知类型的 token: " + std::to_string(type));
  }
#undef caseof
}

} // namespace lumos::token
