#include <lumos.hpp>

namespace lumos {

static const str operator_list[] = {
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

static const str keyword_list[] = {
    "fn",        "if",         "else",       "for",        "while",      "do",
    "break",     "continue",   "return",     "namespace",  "private",    "public",
    "protected", "class",      "struct",     "enum",       "interface",  "trait",
    "impl",      "type",       "using",      "import",     "export",     "as",
    "is",        "in",         "of",         "new",        "delete",     "this",
    "super",     "null",       "true",       "false",      "void",       "bool",
    "char",      "byte",       "short",      "int",        "long",       "float",
    "double",    "string",     "array",      "map",        "set",        "list",
    "tuple",     "variant",    "any",        "auto",       "const",      "static",
    "final",     "virtual",    "override",   "inline",     "extern",     "thread_local",
    "register",  "volatile",   "mutable",    "restrict",   "atomic",     "synchronized",
    "transient", "native",     "abstract",   "sealed",     "readonly",   "template",
    "typename",  "typenameof", "typenameis", "typenamein", "typenameof", "typenameis",
    "typenamein"};

CTX::CTX() {
  for (const auto &op : operator_list) {
    operators.append(op, null);
  }
  for (const auto &kwd : keyword_list) {
    keywords[kwd] = kwd;
  }
}

} // namespace lumos
