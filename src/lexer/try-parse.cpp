#include <lumos.hpp>

namespace lumos::lexer {

// 尝试解析连续的空格
auto Lexer::try_space() -> Token * {
  if (rem == 0) return null;
  if (!isspace(code[0])) return null;
  size_t n = 1;
  for (; isspace(code[n]); n++) {}
  return token(Token::Space, n);
}

// 尝试解析注释
auto Lexer::try_comment() -> Token * {
  if (rem < 2) return null;
  size_t n = 2;

  if (code[0] == '/' && code[1] == '/') { // 单行注释，解析到行尾或结束
    for (; code[n] != '\0' && code[n] != '\n'; n++) {}
    return token(Token::Comment, n);
  }

  if (code[0] == '/' && code[1] == '*') { // 多行注释，解析到 '*/' 或结束
    for (; code[n] != '\0' && (code[n] != '*' || code[n + 1] != '/'); n++) {}
    if (code[n] == '\0') throw Error("多行注释未结束");
    return token(Token::Comment, n + 2);
  }

  return null;
}

// 尝试解析宏
auto Lexer::try_macro() -> Token * {
  if (rem == 0) return null;
  size_t n = 0;
  //- 未实现
  return null;
}

// 尝试解析数字
auto Lexer::try_num() -> Token * {
  if (rem == 0) return null;
  if (!isdigit(code[0]) && (code[0] != '.' || !isdigit(code[1]))) return null;
  size_t n       = 0;
  auto   toktype = Token::Int;
  for (; isdigit(code[n]); n++) {}
  if (code[n] == 'e' || code[n] == 'E') goto exp;
  if (code[n] == '.') {
    toktype = Token::Float;
    for (n++; isdigit(code[n]); n++) {}
    if (code[n] == 'e' || code[n] == 'E') goto exp;
  }
  for (; issymc(code[n]); n++) {}
  goto end;

exp:
  toktype = Token::Float, n++;
  if (code[n] == '+' || code[n] == '-') n++;
  for (; issymc(code[n]); n++) {}

end:
  toktype = std::get<0>(token::num_type_suffix(str(code, n), toktype == Token::Float));
  return token(toktype, n);
}

// 解析个字符串
// xxx"yyy"zzz 这样的形式
static auto _str(cstr code, char c) -> size_t {
  size_t n = 0;
  for (; issymc(code[n]); n++) {}
  if (code[n++] != c) return 0;
  if (n == 2 && (code[0] == 'r' || code[0] == 'R')) {
    // 未实现
  }
  for (; code[n] && (code[n] != c || code[n - 1] == '\\'); n++) {
    if (code[n] == '\n' && code[n - 1] != '\\') throw Error("字符串不能跨行");
  }
  if (code[n++] != c) return 0;
  for (; issymc(code[n]); n++) {}
  return n;
}

// 尝试解析字符串
auto Lexer::try_str() -> Token * {
  if (rem == 0) return null;
  size_t n = _str(code, '"');
  if (n == 0) return null;
  return token(Token::Str, n);
}

// 尝试解析字符
auto Lexer::try_chr() -> Token * {
  if (rem == 0) return null;
  size_t n = _str(code, '\'');
  if (n == 0) return null;
  return token(Token::Chr, n);
}

// 尝试解析运算符
auto Lexer::try_op() -> Token * {
  if (rem == 0) return null;
  const auto [sp, vp] = ctx.operators.match(code);
  if (sp == null) return null;
  return token(Token::Op, sp->length());
}

// 尝试解析属性
// 包括运算符的标识符必须以非数字开头
// 基本词法只允许 A-Z a-z _ $ -
// 扩展的词法允许其它语言的字符
auto Lexer::try_attr() -> Token * {
  if (rem == 0) return null;
  if (code[0] != '@') return null;
  size_t n = 1;
  for (; issymc(code[n]) || code[n] == '-'; n++) {}
  if (n == 1) throw Error("属性名不能为空");
  return token(Token::Attr, n);
}

// 尝试解析
auto Lexer::try_punc() -> Token * {
  if (rem == 0) return null;
  if (code[0] == ':' && code[1] == ':') return token(Token::Punc, 2);
  if (code[0] == '.') return token(Token::Punc, 1);
  if (code[0] == ',') return token(Token::Punc, 1);
  if (code[0] == ';') return token(Token::Punc, 1);
  if (code[0] == '{') return token(Token::Punc, 1);
  if (code[0] == '}') return token(Token::Punc, 1);
  if (code[0] == '[') return token(Token::Punc, 1);
  if (code[0] == ']') return token(Token::Punc, 1);
  if (code[0] == '(') return token(Token::Punc, 1);
  if (code[0] == ')') return token(Token::Punc, 1);
  return null;
}

// 尝试解析标识符
// 标识符必须以非数字开头
// 基本词法只允许 A-Z a-z _ $
// 扩展的词法允许其它语言的字符
auto Lexer::try_sym() -> Token * {
  if (rem == 0) return null;
  if (!issymc(code[0]) || isdigit(code[0])) return null;
  size_t n = 1;
  for (; issymc(code[n]); n++) {}
  str raw = str(code, n);
  if (auto it = ctx.keywords.find(raw); it != ctx.keywords.end())
    return token(Token::Kwd, n, it->second);
  return token(Token::Sym, n, raw);
}

} // namespace lumos::lexer