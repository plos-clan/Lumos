#include <lumos.hpp>

namespace lumos {

static const str ops_list[] = {
    "++",     "--",       "->",                         //
    "!",      "&&",       "||",     "^^",               //
    "~",      "&",        "|",      "^",   "<<",  ">>", //
    "==",     "!=",       "<=",     ">=",  "<",   ">",  //
    "+",      "-",        "*",      "/",   "%",         //
    "?",      ":",                                      //
    "=",      "+=",       "-=",     "*=",  "/=",  "%=", //
    "|=",     "&=",       "^=",     "<<=", ">>=",       //
    "||=",    "&&=",      "^^=",                        //
    "sizeof", "lengthof", "typeof",                     //
};

Ctx::Ctx(const str &file) : file(file) {
  for (const auto &op : ops_list) {
    operators.append(op, null);
  }
}

} // namespace lumos
