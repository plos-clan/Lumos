#include <lumos.hpp>

namespace lumos::lexer {

static Token *const skipped_token = ((Token *)-1); // 表示被跳过的 token
    // 用作中间值, lexer 的输出仍然是实际的 token 或 null

Lexer::Lexer(CTX &ctx, cstr file, void *code, size_t len) : ctx(ctx) {
  if (code == null) throw Error("code 不能为 null");
  if (len == 0) throw Error("len 不能为 0");
  this->file = file;
  this->code = (char *)code;
  this->len  = len;
  this->rem  = len;
}

Lexer::Lexer(CTX &ctx, void *code, size_t len) : Lexer(ctx, "", code, len) {}

auto Lexer::token(EToken type, size_t n, strref raw) -> Token * {
  if (n == 0) throw Error("尝试输出空token");
  if (n > rem) throw Error("输出的token长度大于剩余的字符数");

  const auto [s, pos] = update(n);

  if (!return_space && (type == Token::Space || type == Token::Comment)) return skipped_token;

  auto *tok = token::mktoken(type, raw, pos);

  if (log_tokens) cout << *tok << endl;
  return tok;
}

auto Lexer::token(EToken type, size_t n) -> Token * {
  return token(type, n, str(code, n));
}

auto Lexer::_get() -> Token * {
#define TRY(name)                                                                                  \
  ({                                                                                               \
    Token *tok = try_##name();                                                                     \
    if (tok != null && tok != skipped_token) return tok;                                           \
    tok;                                                                                           \
  })
  if (rem == 0) return null; // 保证 rem 不为 0

  if (instr) {}

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
  TRY(op);
  TRY(attr);
  TRY(rootns);
  TRY(punc);
  TRY(sym);

  if (return_invalid) return token(Token::Inv, 1);
  throw Error("无法识别的字符 " + to_string(code[0]) + ": " + code[0] + " 在" + to_string(line) +
              ':' + to_string(col));

#undef TRY
}

//+ 公开的函数

auto Lexer::eof() const -> bool {
  return rem == 0;
}

void Lexer::skip_space() {
  if (return_space || rem == 0) return;
  while (try_space() != null || try_comment() != null) {}
}

auto Lexer::peekch() const -> char {
  return rem == 0 ? '\0' : code[0];
}

auto Lexer::getch() -> char {
  if (rem == 0) return 0;
  val[s, pos] = update(1);
  return s[0];
}

auto Lexer::peek() -> Token * {
  return _tok ?: (_tok = _get());
}

auto Lexer::peek(EToken type) -> Token * {
  tryed.emplace_back(type, "");
  Token *tok = peek();
  return tok != null && tok->type == type ? tok : null;
}

auto Lexer::peek(strref value) -> Token * {
  tryed.emplace_back(Token::Inv, value);
  Token *tok = peek();
  return tok != null && tok->raw == value ? tok : null;
}

auto Lexer::peek(EToken type, strref value) -> Token * {
  tryed.emplace_back(type, value);
  Token *tok = peek();
  return tok != null && tok->type == type && tok->raw == value ? tok : null;
}

auto Lexer::get() -> PToken {
  tryed.clear();
  if (_tok != null) {
    Token *temp = _tok;
    _tok        = null;
    return temp;
  }
  return _get();
}

auto Lexer::get(EToken type) -> PToken {
  Token *tok = peek(type);
  if (tok == null) return null;
  _tok = null;
  return tok;
}

auto Lexer::get(strref value) -> PToken {
  Token *tok = peek(value);
  if (tok == null) return null;
  _tok = null;
  return tok;
}

auto Lexer::get(EToken type, strref value) -> PToken {
  Token *tok = peek(type, value);
  if (tok == null) return null;
  _tok = null;
  return tok;
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

auto Lexer::getall() -> Vector<Token> {
  Vector<Token> tokens;
  while (PToken tok = get()) {
    tokens.push_back(*tok);
  }
  return tokens;
}

void Lexer::error(str msg) {
  logger.error(*this, "");
  cout << "Lexer: 期望 ";
  for (const auto &[type, value] : tryed) {
    if (type != Token::Inv) cout << type;
    if (type != Token::Inv && value.length() > 0) cout << ':';
    if (value.length() > 0) cout << '`' << value << '`';
    cout << ' ';
  }
  if (const auto *tok = peek(); tok != null) {
    cout << "但是遇到了 " << tok->type << ":`" << tok->raw << '`' << endl;
  } else {
    cout << "但文件已结束" << endl;
  }
  throw Error(msg);
}

} // namespace lumos::lexer
