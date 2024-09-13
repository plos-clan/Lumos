#include <lumos.hpp>

using namespace lumos::lexer;

// constexpr auto example = R"(
// using "std" -> std;

// fn main([int argc, char** argv, [char** envp]]) -> int;

// @assert(argc >= 1)
// fn main(int argc, char* argv[argc]) -> int {
//   std.println("Argc is 1.") if argc == 1;
//   for (var a from 1 to argc) {
//     std.println("Hello, world!");
//   }
//   std.println(123., .123, 123.456);
//   return 0;
// }
// )";

constexpr auto example = R"(
1
123
0x123
123z
123q
123.456q
123.456e+1
0x1e+1
)";

namespace lumos::token {

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

} // namespace lumos::token

auto main() -> int {
  // lumos::Ctx ctx("<stdin>");
  // Lexer      lexer(ctx, example, strlen(example));

  // while (lexer.peek() != null) {
  //   cout << lexer.get() << endl;
  // }

  str s;
  while (1) {
    cin >> s;
    lumos::token::num_data(s).println();
  }

  return 0;
}
