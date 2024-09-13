#include <lumos/lexer.hpp>

namespace lumos::lexer {

static Token *const skipped_token = ((Token *)-1); // 表示被跳过的 token
    // 用作中间值, lexer 的输出仍然是实际的 token 或 null

Lexer::Lexer(Ctx &ctx, cstr file, cstr code, size_t len) : ctx(ctx) {
  if (file == null) throw Error("file 不能为 null");
  if (code == null) throw Error("code 不能为 null");
  if (len == 0) throw Error("len 不能为 0");
  this->file = file;
  this->code = code;
  this->len  = len;
  this->rem  = len;
}

Lexer::Lexer(Ctx &ctx, cstr code, size_t len) : Lexer(ctx, ctx.file.c_str(), code, len) {}

auto Lexer::token(Token::EToken type, size_t n) -> Token * {
  if (n == 0) throw Error("尝试输出空token");
  if (n > rem) throw Error("输出的token长度大于剩余的字符数");

  const auto [s, pos] = update(n);

  if (!return_space && (type == Token::Space || type == Token::Comment)) return skipped_token;

  auto *tok = token::mktoken(type, str(s, n), pos);

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
  throw Error("无法识别的字符");

#undef TRY
}

//+ 公开的函数

auto Lexer::eof() const -> bool {
  return rem == 0;
}

auto Lexer::peek_char() const -> byte {
  return rem == 0 ? 0 : code[pos];
}

auto Lexer::get_char() -> byte {
  if (rem == 0) return 0;
  const auto [s, pos] = update(1);
  return s[0];
}

auto Lexer::peek() -> Token * {
  return _tok ?: (_tok = _get());
}

auto Lexer::get() -> Token * {
  if (_tok != null) {
    Token *temp = _tok;
    _tok        = null;
    return temp;
  }
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