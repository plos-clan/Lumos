#include <lumos/lexer.hpp>

namespace lumos::token {

auto operator<<(ostream &os, Token::EToken t) -> ostream & {
  static str lut[] = {
      "Inv",
      //
      "Space", "Comment",
      //
      "Num", "Int", "Uint", "Float", "Fixed",
      //
      "Str", "Chr", "Op", "Attr", "Punc",
      //
      "Sym", "Kwd",
      //
  };

  if (t < 0 || t >= Token::EToken_cnt) throw Error("EToken超出范围");
  os << lut[t];
  return os;
}

// 输出 token 信息
auto operator<<(ostream &os, const Token &tok) -> ostream & {
  os << "<token " << tok.type << " str='" << tok.s << "'";
  if (tok.line > 0) os << " line=" << tok.line;
  if (tok.col > 0) os << " col=" << tok.col;
  os << '>';
  return os;
}

} // namespace lumos::token
