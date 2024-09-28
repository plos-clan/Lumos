from pygments.lexer import RegexLexer, words, bygroups
from pygments.token import Keyword, Name, String, Number, Punctuation, Operator, Comment, Whitespace, Generic


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
          (r'`', String, 'fmtstr'),
          (r'"([^"\\]|\\.)*"|\'([^\'\\]|\\.)*\'', String),
          (chars('([{'), Punctuation, 'root'),
          (chars(')]}'), Punctuation, '#pop'),
          (chars(',;'), Punctuation),
          (chars('+-*/%=^&|?:<>!~'), Operator),
          (kwds(['sizeof', 'lengthof', 'typeof', 'typenameof']), Operator),
          (r'# *(define|undef|if|fidef|ifndef|end|set|clear|once|include)', Generic.Prompt),
          (kwds([
              ['void', 'bool', 'char', 'int', 'uint', 'float'],
              ['i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64'],
              ['f16', 'f32', 'f64', 'f128'],
              ['size_t', 'ssize_t'],
          ]), Name.Class),
          (kwds(['true', 'false', 'null', 'undefined']), Number),
          (kwds([
              ['var', 'val', 'obj', 'let', 'lit'],
              ['try', 'catch', 'throw'],
              ['for', 'while', 'do', 'if', 'else', 'elif'],
              ['switch', 'case', 'default'],
              ['in', 'of', 'as', 'from', 'to', 'using'],
              ['break', 'continue', 'leave', 'breaked', 'then', 'goto', 'return'],
              ['namespace', 'struct', 'class', 'enum', 'union'],
              ['typename'],
          ]), Keyword),
          (r'[A-Z][a-zA-Z0-9_]*', Name.Class),
          (r'(fn)(\s|\n|\r)*([a-zA-Z_][a-zA-Z0-9_]*)', bygroups(Keyword, Whitespace, Name.Function)),
          (r'([a-zA-Z_][a-zA-Z0-9_]*)(\s|\n|\r)*(\()', bygroups(Name.Function, Whitespace, Punctuation)),
          (r'fn', Keyword),
          (kwds(['pow', 'sqrt']), Operator),
          (r'[a-z_][a-zA-Z0-9_]*', Name),
          (r'@[a-zA-Z_][a-zA-Z0-9_\-]*', Name.Attribute),
          (r'@[^ \t\r\n\<\>\(\)\[\]\{\}\,\;]*', Name.Attribute),
          (r'\$[a-zA-Z0-9_]+', Name.Attribute),
          (r'.', Comment.Special),
      ],
      'fmtstr': [
          (r'[^`\$\\]|\\.', String),
          (r'(\$)([a-zA-Z_][a-zA-Z0-9_]*)', bygroups(Keyword, Name)),
          (r'(\$)(\{)', bygroups(Keyword, Punctuation), 'root'),
          (r'\$\$', Keyword),
          (r'\$', String),
          (r'`', String, '#pop'),
      ],
  }
