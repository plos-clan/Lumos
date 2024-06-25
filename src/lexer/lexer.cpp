#include <lumos/lexer.hpp>

namespace lumos::lexer {

static Token *const skipped_token = ((Token *)-1); // 表示被跳过的 token
    // 用作中间值, lexer 的输出仍然是实际的 token 或 null

Lexer::Lexer(const str &code) : code(code.c_str()) {
  rem = len = code.length();
}

Lexer::Lexer(cstr code) : code(code) {
  if (code == null) throw Error("code 不能为 null");
  rem = len = strlen(code);
}

Lexer::Lexer(cstr code, size_t len) : code(code), len(len), rem(len) {
  if (code == null) throw Error("code 不能为 null");
}

auto Lexer::token(Token::EToken type, size_t n) -> Token * {
  if (n == 0) throw Error("尝试输出空token");
  if (n > rem) throw Error("Lexer: 输出的token长度大于剩余的字符数");

  u32 _pos = this->pos, _line = this->line, _col = this->col;

  // 更新指针位置
  cstr s  = code;
  code   += n;
  pos    += n;
  rem    -= n;

  // 更新行数和列数
  u32 col = this->col + n;
  for (size_t i = 0; i < n; i++) {
    if (s[i] == '\n') {
      line++;
      col = n - i;
    }
  }
  this->col = col;

  // 跳过空格与注释
  if (!return_space && type == Token::Space) return skipped_token;
  if (!return_space && type == Token::Comment) return skipped_token;

  // 创建 token 并返回
  auto *tok = new Token(type, str(s, n), _pos, _line, _col);
  if (log_tokens) cout << *tok << endl;
  return tok;
}

auto Lexer::_get() -> Token * {
#define TRY(name)                                                                                  \
  ({                                                                                               \
    Token *tok = try_##name();                                                                     \
    if (tok != null && tok != skipped_token) return tok;                                           \
    tok;                                                                                           \
  })

  if (rem == 0) return null; // 保证 rem 不为 0

  // 先处理空格和注释
  if (return_space) {
    TRY(space);
    TRY(comment);
  } else {
    while (TRY(space) || TRY(comment)) {} // 跳过所有空格和注释
    if (rem == 0) return null;            // 保证 rem 不为 0
  }

  // 然后处理宏
  TRY(macro);

  TRY(num);
  TRY(str);
  TRY(chr);
  TRY(op);
  TRY(attr);
  TRY(punc);
  TRY(sym);

  if (return_invalid) return token(Token::Inv, 1);

#undef TRY
  return null;
}

//+ 公开的函数

auto Lexer::eof() const -> bool {
  return rem == 0;
}

auto Lexer::peek_char() const -> byte {
  if (rem == 0) return 0;
  return code[pos];
}

auto Lexer::get_char() -> byte {
  if (rem == 0) return 0;
  char c = code[0];
  code++;
  pos++;
  rem--;
  return c;
}

auto Lexer::peek() -> PToken {
  if (_tok) return _tok;
  return _tok = _get();
}

auto Lexer::get() -> PToken {
  if (_tok) return std::move(_tok);
  return _get();
}

auto Lexer::get_block() -> cstr {
  if (rem < 2) return null;
  char c = code[0];
  if (c != '(' && c != '[' && c != '{') return null;
  Stack<char> stack;
  stack.push(c);
  size_t n = 0;
  while (!stack.empty() && (c = code[++n])) {
    if (c == '(' || c == '[' || c == '{') stack.push(c);
    if (c == ')') {
      if (stack.top() != '(') throw Error("括号不匹配");
      stack.pop();
    }
    if (c == ']') {
      if (stack.top() != '[') throw Error("括号不匹配");
      stack.pop();
    }
    if (c == '}') {
      if (stack.top() != '{') throw Error("括号不匹配");
      stack.pop();
    }
  }
  if (c == '\0') throw Error("括号不匹配");
  cstr s  = strndup(code + 1, n - 1);
  code   += n + 1;
  return s;
}

} // namespace lumos::lexer