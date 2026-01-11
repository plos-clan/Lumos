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
          (r'\/\/.*?(\n|$)|\/\*(.|\n)*?\*\/', Comment),
          (r'\\fn> *([a-zA-Z0-9_]+)', bygroups(Name.Function)),
          (r'\\def> *([a-zA-Z0-9_]+)', bygroups(Name.Function)),
          (r'\\fun> *([a-zA-Z0-9_]+)', bygroups(Name.Function)),
          (r'\\act> *([a-zA-Z0-9_]+)', bygroups(Name.Function)),
          (r'\\var> *([a-zA-Z0-9_]+)', bygroups(Name)),
          (r'\\val> *([a-zA-Z0-9_]+)', bygroups(Name.Constant)),
          (r'\\op> *([a-zA-Z0-9_]+)', bygroups(Operator)),
          (r'\\type> *([a-zA-Z0-9_]+)', bygroups(Name.Class)),
          (r'\\num> *([a-zA-Z0-9_]+)', bygroups(Number)),
          (r'\\kwd> *([a-zA-Z0-9_]+)', bygroups(Keyword)),
          (r'\\(\[.*?\])', bygroups(Generic.Output)),
          (r'(act)(\s*)(\[)', bygroups(Keyword, Whitespace, Punctuation), 'perm'),
          (r'(with)(\s*)(\[)', bygroups(Keyword, Whitespace, Punctuation), 'capture'),
          (r'</', Punctuation, 'generic'),
          (r'([0-9][a-zA-Z0-9_]*\.?|\.[0-9])[a-zA-Z0-9_]*', Number),
          (r'`', String, 'fmtstr'),
          (r'"([^"\\]|\\.)*"|\'([^\'\\]|\\.)*\'', String),
          (chars('([{'), Punctuation, 'root'),
          (chars(')]}'), Punctuation, '#pop'),
          (chars(',;'), Punctuation),
          (chars('+-*/%=^&|?:<>!~.'), Operator),
          (kwds(['sizeof', 'lengthof', 'typeof', 'typenameof', 'is', 'dbg']), Operator),
          (r'(# *define|# *undef)( *)([a-zA-Z0-9_]*)', bygroups(Generic.Prompt, Whitespace, Number)),
          # (r'# *(define|undef|if|fidef|ifndef|end|set|clear|once|include)', Generic.Prompt),
          (r'# *[a-zA-Z0-9_\-]*', Generic.Prompt),
          (kwds([
              ['unit', 'bool', 'char', 'int', 'uint', 'float', 'byte'],
              ['i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64'],
              ['f16', 'f32', 'f64', 'f128'],
              ['b8', 'b16', 'b32', 'b64'],
              ['usize', 'isize'],
              ['str'],
          ]), Name.Class),
          (kwds(['true', 'false', 'null', 'undefined', 'noexcept']), Number),
          (r'(goto|break|continue)(\s|\n|\r)*([a-zA-Z_][a-zA-Z0-9_]*)', bygroups(Keyword, Whitespace, Name.Constant)),
          (kwds([
              ['var', 'val', 'imv', 'fin', 'obj', 'let', 'lit', 'ref', 'restrict'],
              ['late', 'lazy'],
              ['try', 'catch', 'throw'],
              ['for', 'while', 'do', 'if', 'else', 'elif'],
              ['switch', 'match'],
              ['as', 'from', 'to', 'using', 'in', 'of', 'with', 'by', 'where', 'yields', 'aka'],
              ['break', 'continue', 'leave', 'breaked', 'then', 'goto', 'return'],
              ['async', 'await'],
              ['namespace', 'struct', 'class', 'enum', 'union', 'impl', 'template', 'measure', 'permission'],
              ['typename', 'variant', 'lateinit', 'initvar'],
              ['assert', 'assume'],
              ['inttype', 'floattype', 'numtype'],
          ]), Keyword),
          (kwds(['std']), Name.Class),
          (r'[A-Z][A-Z0-9_]*(?=[^a-z])', Name.Constant),
          (r'[A-Z][a-zA-Z0-9_]*', Name.Class),
          (r'(def|fun|act)(\s|\n|\r)*([a-zA-Z_][a-zA-Z0-9_]*)', bygroups(Keyword, Whitespace, Name.Function)),
          (r'(def|fun|act)', Keyword),
          (r'([a-zA-Z_][a-zA-Z0-9_]*)(\s|\n|\r)*(\()', bygroups(Name.Function, Whitespace, Punctuation)),
          (kwds(['pow', 'sqrt']), Operator),
          (r'[a-zA-Z_][a-zA-Z0-9_]*:', Name.Constant),
          (r'[a-z_][a-zA-Z0-9_]*', Name),
          (r'@[a-zA-Z_][a-zA-Z0-9_\-]*', Name.Attribute),
          (r'@[^ \t\r\n\<\>\(\)\[\]\{\}\,\;]*', Name.Attribute),
          (r'\$[a-zA-Z0-9_]+', Name.Attribute),
          (r'\\[a-zA-Z0-9_]+', Operator),
          (r'[^ \t\r\n\<\>\(\)\[\]\{\}!@#$%^&*,./?:;\'\"`~|\\+=\-]+', Comment.Special),
      ],
      'fmtstr': [
          (r'[^`\$\\]|\\.', String),
          (r'(\$)([a-zA-Z_][a-zA-Z0-9_]*)', bygroups(Keyword, Name)),
          (r'(\$)(\{)', bygroups(Keyword, Punctuation), 'root'),
          (r'\$\$', Keyword),
          (r'\$', String),
          (r'`', String, '#pop'),
      ],
      'perm': [
          (r'\s+', Whitespace),
          (r'\]', Punctuation, '#pop'),
          (r'[\{\}]', Punctuation),
          (r'[,]', Punctuation),
          (r'[+\-]', Operator),
          (r'%+', Name.Attribute),
          (r'[a-zA-Z_][a-zA-Z0-9_]*(?:\.[a-zA-Z0-9_]+)*', Name.Attribute),
          (r'[^ \t\r\n\<\>\(\)\[\]\{\}!@#$%^&*,./?:;\'\"`~|\\+=\-]+', Comment.Special),
      ],
      'capture': [
          (r'\s+', Whitespace),
          (r'\]', Punctuation, '#pop'),
          (r'[,]', Punctuation),
          (r'[=&]', Operator),
          (r'[a-zA-Z_][a-zA-Z0-9_]*', Name),
          (r'[^ \t\r\n\<\>\(\)\[\]\{\}!@#$%^&*,./?:;\'\"`~|\\+=\-]+', Comment.Special),
      ],
      'generic': [
          (r'\s+', Whitespace),
          (r'/?>', Punctuation, '#pop'),
          (r'[,]', Punctuation),
          (kwds(['typename']), Keyword),
          (r'\.\.\.', Operator),
          (r'[0-9]+', Number),
          (r'[a-zA-Z_][a-zA-Z0-9_]*', Name.Class),
          (r'[^ \t\r\n\<\>\(\)\[\]\{\}!@#$%^&*,./?:;\'\"`~|\\+=\-]+', Comment.Special),
      ],
  }
