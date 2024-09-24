from pygments.lexer import RegexLexer, words, bygroups
from pygments.token import Text, Keyword, Name, String, Number, Punctuation, Operator, Comment, Whitespace, Generic


def kwds(lists):
  if isinstance(lists[0], list):
    lists = [r for l in lists for r in l]
  return words(lists, prefix=r'\b', suffix=r'\b')


def chars(s):
  return words([c for c in s])


class LumosLexer(RegexLexer):
  name = 'Lumos'
  aliases = ['lumos', 'lm']
  filenames = ['*.lm']

  tokens = {
      'root': [
          (r'(\s|\n|\r)+', Whitespace),
          (r'\/\/.*?(\n|$)|\/\*.*?\*\/', Comment),
          (r'([0-9][a-zA-Z0-9_]*\.?|\.[0-9])[a-zA-Z0-9_]*', Number),
          (r'([a-zA-Z_\$][a-zA-Z0-9_\$]*)?"([^"\\]|\\.)*"([a-zA-Z_\$][a-zA-Z0-9_\$]*)?', String),
          (r'([a-zA-Z_\$][a-zA-Z0-9_\$]*)?\'([^\'\\]|\\.)*\'([a-zA-Z_\$][a-zA-Z0-9_\$]*)?', String),
          (chars('()[]{},;'), Punctuation),
          (chars('+-*/%=^&|?:<>!~'), Operator),
          (kwds(['sizeof', 'lengthof', 'typeof']), Operator),
          (r'# *(define|undef|if|fidef|ifndef|set|clear)', Generic.Prompt),
          (kwds([
              ['void', 'int', 'uint', 'float'],
              ['i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64'],
              ['f16', 'f32', 'f64', 'f128'],
          ]), Name.Class),
          (kwds([
              ['var', 'val', 'const', 'obj', 'let', 'constexpr'],
              ['try', 'catch', 'throw'],
              ['for', 'while', 'do', 'if', 'else', 'elif'],
              ['in', 'of', 'as', 'from', 'to', 'using'],
              ['break', 'continue', 'leave', 'breaked', 'then', 'goto', 'return'],
              ['namespace', 'struct', 'class', 'enum', 'union'],
          ]), Keyword),
          (r'[A-Z][a-zA-Z0-9_]*', Name.Class),
          (r'(fn)(\s|\n|\r)*([a-zA-Z_][a-zA-Z0-9_]*)', bygroups(Keyword, Whitespace, Name.Function)),
          (r'([a-zA-Z_][a-zA-Z0-9_]*)(\s|\n|\r)*(\()', bygroups(Name.Function, Whitespace, Punctuation)),
          (r'fn', Keyword),
          (kwds(['pow', 'sqrt']), Operator),
          (r'[a-z_][a-zA-Z0-9_]*', Name),
          (r'@[a-zA-Z_][a-zA-Z0-9_\-]*', Name.Attribute),
          (r'@[^a-zA-Z_][^ \t\r\n\(\)\[\]\{\}\,\;]*', Name.Attribute),
          (r'.', Comment.Special),
      ]
  }


def get_lexer_by_name(_alias, **options):
  if _alias == 'lumos': return LumosLexer(**options)
  return real_get_lexer_by_name(_alias, **options)


from pygments import lexers

real_get_lexer_by_name = lexers.get_lexer_by_name
lexers.get_lexer_by_name = get_lexer_by_name
