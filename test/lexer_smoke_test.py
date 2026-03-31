from pathlib import Path
import sys

from pygments import lex

ROOT = Path(__file__).resolve().parent.parent
if str(ROOT) not in sys.path:
  sys.path.insert(0, str(ROOT))

from scripts.lumos import LumosLexer


def token_values(code: str):
  return [(str(tok), val) for tok, val in lex(code, LumosLexer())]


def run_case(name: str, code: str):
  print(f"=== {name} ===")
  print(code)
  for tok, val in token_values(code):
    show = val.replace("\n", "\\n")
    print(f"{tok:<24} {show}")
  print()


def has_identifier_after_closer(code: str, target: str) -> bool:
  seq = token_values(code)
  return any(tok == "Token.Name" and val == target for tok, val in seq)


def has_keyword_token(code: str, keyword: str) -> bool:
  seq = token_values(code)
  return any(tok == "Token.Keyword" and val == keyword for tok, val in seq)


def main():
  cases = {
      "dollar_init_nested_paren": "${.x = f(1)} tail",
      "dollar_array_nested_paren": "$[foo(bar(1))] tail",
      "dollar_init_nested_bracket": "${.x = arr[0]} tail",
      "perm_capture": "act[fs.read + %perm] with[a=&b] fun x() {}",
  }

  for name, code in cases.items():
    run_case(name, code)

  checks = {
      "tail_token_after_dollar_init": has_identifier_after_closer("${.x = f(1)} tail", "tail"),
      "tail_token_after_dollar_array": has_identifier_after_closer("$[foo(bar(1))] tail", "tail"),
      "def_is_keyword_after_dollar_init": has_keyword_token("${.x = f(1)} def foo() {}", "def"),
      "def_is_keyword_after_dollar_array": has_keyword_token("$[foo(bar(1))] def foo() {}", "def"),
  }

  print("=== checks ===")
  failed = []
  for key, ok in checks.items():
    status = "PASS" if ok else "FAIL"
    print(f"{key}: {status}")
    if not ok:
      failed.append(key)

  if failed:
    print("\nFAILED:", ", ".join(failed))
    raise SystemExit(1)

  print("\nAll checks passed.")


if __name__ == "__main__":
  main()
