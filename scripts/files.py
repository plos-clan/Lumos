from pygments.lexer import RegexLexer, bygroups
from pygments.token import Keyword, Name, Operator, Whitespace, Punctuation, Comment


class FilesLexer(RegexLexer):
  name = 'Files'
  aliases = ['files']
  filenames = []

  tokens = {
      'root': [
          (r' +', Whitespace),
          (r'#.*$', Comment),
          (r'([^ ]+)(/)', bygroups(Operator, Punctuation)),
          (r'[^ ]+\.lm', Name.Attribute),
          (r'[^ ]+\.lh', Keyword),
          (r'[^ ]+\.yaml', Name.Class),
          (r'[^ ]+', Name.Function),
      ],
  }
