#include <lumos/lexer.hpp>

namespace lumos::lexer {

// 尝试解析连续的空格
auto Lexer::try_space() -> Token * {
  if (rem == 0) return null;
  size_t n = 0;
  while (lut[code[n]] == lut_space)
    n++;
  if (n == 0) return null;
  return token(Token::Space, n);
}

// 尝试解析注释
auto Lexer::try_comment() -> Token * {
  if (rem < 2) return null;
  size_t n = 2;

  if (code[0] == '/' && code[1] == '/') { // 单行注释，解析到行尾或结束
    while (code[n] != '\0' && code[n] != '\n')
      n++;
    return token(Token::Comment, n);
  }

  if (code[0] == '/' && code[1] == '*') { // 多行注释，解析到 '*/' 或结束
    while (code[n] != '\0' && (code[n] != '*' || code[n + 1] != '/'))
      n++;
    if (code[n] != '\0') n += 2;
    return token(Token::Comment, n);
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
  if (code[0] < '0' || '9' < code[0]) return null;
  size_t n = 1;
  while (lut[code[n]] == lut_sym)
    n++;
  return token(Token::Num, n);
}

// 解析个字符串
// xxx"yyy"zzz 这样的形式
static auto _str(cstr code, char c) -> size_t {
  size_t n = 0;
  while (lut[code[n]] == lut_sym)
    n++;
  if (code[n++] != c) return 0;
  while (code[n] && (code[n] != c || code[n - 1] == '\\'))
    n++;
  if (code[n++] != c) return 0;
  while (lut[code[n]] == lut_sym)
    n++;
  return n;
}

// 尝试解析字符串
auto Lexer::try_str() -> Token * {
  if (rem == 0) return null;
  size_t n = _str(code + n, '"');
  if (n == 0) return null;
  return token(Token::Str, n);
}

// 尝试解析字符
auto Lexer::try_chr() -> Token * {
  if (rem == 0) return null;
  size_t n = _str(code + n, '\'');
  if (n == 0) return null;
  return token(Token::Chr, n);
}

// 尝试解析运算符
auto Lexer::try_op() -> Token * {
  if (rem == 0) return null;
  size_t n = 1;
  while (ops_list.find(str(code, n)) != ops_list.end())
    n++;
  if (--n == 0) return null;
  return token(Token::Op, n);
}

// 尝试解析属性
auto Lexer::try_attr() -> Token * {
  if (rem == 0) return null;
  if (code[0] != '@') return null;
  // 包括运算符的标识符必须以非数字开头
  // 基本词法只允许 A-Z a-z _ $ 和运算符符号
  // 扩展的词法允许其它语言的字符
  if ((lut[code[1]] != lut_sym && lut[code[1]] != lut_op) || ('0' <= code[0] && code[0] <= '9'))
    return null;
  size_t n = 2;
  while (lut[code[n]] == lut_sym || lut[code[n]] == lut_op)
    n++;
  return token(Token::Attr, n);
}

// 尝试解析
auto Lexer::try_punc() -> Token * {
  if (rem == 0) return null;
  size_t n = 1;
  while (puncs_list.find(str(code, n)) != puncs_list.end())
    n++;
  if (--n == 0) return null;
  return token(Token::Punc, n);
}

// 尝试解析标识符
auto Lexer::try_sym() -> Token * {
  if (rem == 0) return null;
  // 标识符必须以非数字开头
  // 基本词法只允许 A-Z a-z _ $
  // 扩展的词法允许其它语言的字符
  if (lut[code[0]] != lut_sym || ('0' <= code[0] && code[0] <= '9')) return null;
  size_t n = 1;
  while (lut[code[n]] == lut_sym)
    n++;
  return token(Token::Sym, n);
}

} // namespace lumos::lexer