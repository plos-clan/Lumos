#include <lumos.hpp>

namespace lumos::token {

static constexpr auto tab_indent = 4;

auto operator<<(ostream &os, Token::EToken t) -> ostream & {
  static const char *lut[] = {
      "Inv",       //
      "Space",     //
      "Comment",   //
      "Int",       //
      "Float",     //
      "Fixed",     //
      "MPZ",       //
      "MPQ",       //
      "Str",       //
      "Chr",       //
      "FmtStrBeg", //
      "FmtStrEnd", //
      "Op",        //
      "Attr",      //
      "RootNS",    //
      "Punc",      //
      "Sym",       //
      "Kwd",       //
  };
  if (t < 0 || t >= Token::Cnt) throw Error("EToken 超出范围");
  return os << lut[t];
}

Token::Token(EToken type, strref raw, TokenPosRef pos) : TokenPos(pos), type(type), raw(raw) {}

auto Token::is(EToken type) const -> bool {
  throw this->type == type;
}

void Token::_print_to(ostream &os) const {
  // do nothing
}

// 输出 token 信息
void Token::print_to(ostream &os) const {
  os << "<token " << type;
  _print_to(os);
  os << " raw='" << raw << "'";
  if (line > 0 && col > 0) os << " pos=" << line << ':' << col;
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

// 将字符串转义
static auto unescape(str raw) -> str {
  strbuilder sb;
  for (size_t i = 0; i < raw.length(); i++) {
    if (raw[i] != '\\') {
      sb.append(raw[i]);
      continue;
    }
    switch (({
      char c = raw[++i];
      'A' <= c &&c <= 'Z' ? c - 'A' + 'a' : c;
    })) {
    case '\r':
      if (raw[i + 1] == '\n') i++;
      break;
    case '\n': break;
    case 'a': sb += '\a'; break;
    case 'b': sb += '\b'; break;
    case 'e': sb += '\e'; break;
    case 'f': sb += '\f'; break;
    case 'n': sb += '\n'; break;
    case 'r': sb += '\r'; break;
    case 't': sb += '\t'; break;
    case 'v': sb += '\v'; break;
    case 'x': {
      char *end = null;
      char  c   = strtol(raw.substr(i + 1, 2).c_str(), &end, 16);
      if (end == null) throw Error("字符串字面量的十六进制转义字符不合法");
      sb += c;
      i  += 2;
      break;
    }
    case 'u': {
      char *end = null;
      int   n   = 6;
      u32   c   = strtol(raw.substr(i + 1, 6).c_str(), &end, 16);
      if (end == null) n = 4, c = strtol(raw.substr(i + 1, 4).c_str(), &end, 16);
      if (end == null) throw Error("字符串字面量的 Unicode 转义字符不合法");
      char      buf[MB_CUR_MAX];
      mbstate_t state = {};
      size_t    ret   = wcrtomb(buf, c, &state);
      sb.append(buf, ret);
      i += n;
      break;
    }
    default:
      if ('0' <= raw[i] && raw[i] <= '7') { // 八进制转义字符
        char c = raw[i] - '0';
        if ('0' <= raw[i + 1] && raw[i + 1] <= '7') { // 2 位八进制
          c = c * 8 + raw[++i] - '0';
          if ('0' <= raw[i + 1] && raw[i + 1] <= '7') { // 3 位八进制
            c = c * 8 + raw[++i] - '0';
          }
        }
        sb += c;
      } else if (raw[i] == '8' || raw[i] == '9') {
        throw Error("字符串字面量的八进制转义字符不合法");
      } else {
        static const str esc = "'\"\\";
        if (!esc.contains(raw[i])) logger.warn("未知的转义字符: " + raw[i]);
        sb += raw[i];
      }
      break;
    }
  }
  return sb.str();
}

// 普通字符串 "xxx"
static auto normal_string(str raw) -> str {
  return unescape(raw.substr(1, raw.length() - 2));
}

// 原始字符串 '''xxx'''
static auto raw_string(str raw) -> str {
  if (raw.at(0) != '\n' && raw.at(-1) != '\n') throw Error("原始字符串必须以换行开头和结尾");
  return raw.substr(1, raw.length() - 2);
}

// 多行字符串 """xxx"""
static auto multiline_string(str raw) -> str {
  raw = raw.substr(3, raw.length() - 6);
  if (!raw.contains('\n')) return raw; // 单行的就直接返回原始字符串
  strbuilder sb;
  bool       first = true;
  for (str line : raw.lines()) {
    for (size_t i = 0; i < line.length(); i++) {
      if (!isspace(line[i])) {
        line = line.substr(i + 1);
        goto L1;
      }
    }
  L1:
    str data = unescape(line.substr(2));
    if (first) {
      first = false;
      if (line[0] == '|' && line[1] == ' ') {
        sb += data;
      } else {
        throw Error("多行字符串的第一行必须以 `|` 开头并加空格");
      }
    } else if (line[0] == '|' && line[1] == ' ') {
      sb += '\n';
      sb += data;
    } else if (line[0] == '+' && line[1] == ' ') {
      sb += data;
    } else if (line[0] == '*' && line[1] == ' ') {
      sb += ' ';
      sb += data;
    } else {
      throw Error("多行字符串的每行必须以 `|` `+` `*` 开头并加空格");
    }
  }
  return sb.str().substr(1);
}

Str::Str(strref s, TokenPosRef pos) : Token(Token::Str, s, pos) {
  if (raw.length() < 2) throw Fail("字符串字面量长度不足");
  if (raw.length() == 2) return;
  if (raw[0] == '\'') {
    value = raw_string(raw);
  } else if (raw[1] == '\"') {
    value = multiline_string(raw);
  } else {
    value = normal_string(raw);
  }
}

void Str::_print_to(ostream &os) const {
  os << " value='" << value << "'";
}

Chr::Chr(strref s, TokenPosRef pos) : Token(Token::Chr, s, pos) {
  if (raw.length() < 2) throw Fail("字符串字面量长度不足");
  if (raw.length() == 2) return;
  str chs = unescape(raw.substr(1, raw.length() - 2));
  value   = (u32)chs.at(0) | (u32)chs.at(1) << 8 | (u32)chs.at(2) << 16 | (u32)chs.at(3) << 24;
}

void Chr::_print_to(ostream &os) const {
  os << " value='" << value << "'";
}

//* ----------------------------------------------------------------------------------------------------
//; 数字字面量
//* ----------------------------------------------------------------------------------------------------

// 解析数字后缀
auto num_type_suffix(str raw, bool is_float) -> Tuple<Token::EToken, int, int> {
  if (raw.length() == 0) return {is_float ? Token::Float : Token::Int, 0, 0};
  auto it = raw.rbegin();
  if (*it == 'z' || *it == 'Z') return {Token::MPZ, 0, 1};
  if (*it == 'q' || *it == 'Q') return {Token::MPQ, 0, 1};
  if (!isdigit(*it)) goto no_suffix;
  for (; it != raw.rend() && isdigit(*it); it++) {}
  if (it != raw.rend()) {
    auto type = Token::Inv;
    if (*it == 'i' || *it == 'I') type = Token::Int;
    if (*it == 'u' || *it == 'U') type = Token::Int;
    if (is_float && (*it == 'f' || *it == 'F')) type = Token::Float;
    if (type != Token::Inv) {
      str suffix = str(it.base() - 1, raw.end());
      int nbits  = atoi(suffix.substr(1).c_str());
      return {type, nbits, suffix.length()};
    }
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
  if (b == 3 && raw.length() == 2) b = 2;
  if (b == 3) throw Error("重复的进制标记");
  if (b == 2) int_part = mpz(raw.substr(0, raw.length() - 1), base);
  if (b == 1) int_part = mpz(raw.substr(2), base);
  return;
}
_float: { // 浮点字面量，此时整数和小数部分应该已经被删除
  if (raw.length() > 0 && (raw[0] == 'e' || raw[0] == 'E')) { // 解析指数部分
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
//; 标识符
//* ----------------------------------------------------------------------------------------------------

Sym::Sym(strref raw, TokenPosRef pos) : Token(Token ::Sym, raw, pos) {
  if (raw.at(0) == '_' && raw.at(1) == '_' && raw.at(-2) == '_' && raw.at(-1) == '_') {
    reserved = true;
  }
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
  default: throw Fail("无法创建未知类型的 token: " + to_string(type));
  }
#undef caseof
}

} // namespace lumos::token
