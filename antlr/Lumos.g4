grammar Lumos;

SYM: [a-zA-Z_][0-9a-zA-Z_]*;
sym: '::'? SYM ('.' SYM)*;

ATTR: '@' SYM;

OP_ADD: '+';
OP_SUB: '-';
OP_MUL: '*';
OP_DIV: '/';
OP_MOD: '%';
OP_XOR: '^';
OP_INC: '++';
OP_DEC: '--';

PUNC: [+\-*/=];

// 关键字
USING: 'using';
RETURN: 'return';
BREAK: 'break';
CONTINUE: 'continue';
ASSUME: 'assume';
WHERE: 'where';
AS: 'as';

fragment KWD: '#$' [0-9a-zA-Z_]*;

fragment OP: [!@#$%^&*|~:<>?];

WS: [ \t\r]+ -> channel(HIDDEN);

LF: '\n' -> channel(HIDDEN);

BlockComment: '/*' .*? '*/' -> channel(HIDDEN);
LineComment: '//' ~[\n]* -> channel(HIDDEN);

// operators
OP_EQ: '==';
OP_NE: '!=';
OP_GT: '>';
OP_GE: '>=';
OP_LT: '<';
OP_LE: '<=';
OP_AND: '&&';
OP_OR: '||';
OP_NOT: '!';

// numbers

fragment NUM_BASE: [0-9]+;

fragment INT_BIN: '0b' [01]+;
fragment INT_OCT: '0' [0-7]+;
fragment INT_DEC: '0' | [1-9] [0-9]*;
fragment INT_HEX: ('0x' | '0X') [0-9a-fA-F]+;

INT: INT_BIN | INT_OCT | INT_DEC | INT_HEX;

fragment FLOAT_BASE: [0-9]+ '.' [0-9]* | '.' [0-9]+;
fragment FLOAT_EXP: [Ee] [+-]? [0-9]+;
FLOAT: NUM_BASE FLOAT_EXP | FLOAT_BASE FLOAT_EXP?;
float: FLOAT;

IMAG: (NUM_BASE | FLOAT_BASE) 'i' FLOAT_EXP?;
QUAT_J: (NUM_BASE | FLOAT_BASE) 'j' FLOAT_EXP?;
QUAT_K: (NUM_BASE | FLOAT_BASE) 'k' FLOAT_EXP?;
imag: IMAG;
quat_j: QUAT_J;
quat_k: QUAT_K;

num: INT | float | imag | quat_j | quat_k;

STRING_ESCAPE:
	'u' [0-9a-fA-F]{4}
	| 'U' [0-9a-fA-F]{8}
	| 'x' [0-9a-fA-F]{2}
	| .;
STRING: '"' ('\\' STRING_ESCAPE | ~["\\])*? '"';
CHAR: '\'' ( '\\' STRING_ESCAPE | ~['\\])*? '\'';

raw_string: '\'\'\'' .*? '\'\'\'';

REGEX: '/' ~[/\\]* ( '\\' . | ~[/\\])*? '/';

literal: num | STRING | CHAR;

prefix_op: OP_ADD | OP_SUB | OP_INC | OP_DEC | OP_NOT;

suffix_op: OP_INC | OP_DEC | '[' expr ']' | '(' expr ')';

binary_op:
	OP_ADD
	| OP_SUB
	| OP_MUL
	| OP_DIV
	| OP_MOD
	| OP_XOR
	| OP_EQ
	| OP_NE
	| OP_GT
	| OP_GE
	| OP_LT
	| OP_LE
	| OP_AND
	| OP_OR;

// 1, 2, 3
val_list: expr (',' expr)* ','?;
// int a where a > 0 = 1, int b, int c
var_decl: type SYM (WHERE expr)? ('=' expr)?;
var_list: var_decl (',' var_decl)* ','?;
// .xxx = 1, 2, .zzz = 3
elm_list: ('.' SYM '=')? expr (',' ('.' SYM '=')? expr)* ','?;
// xxx = 1, yyy, zzz
enum_list: SYM ('=' expr)? (',' SYM ('=' expr)?)* ','?;

capture_list: '&'? expr (',' '&'? expr)* ','? | '*' | '&';

map: '{' val_list? '}';
struct_init: '{' ('.' SYM '=')? val_list? '}';
list: '[' val_list? ']';
tuple: '(' val_list? ')';

expr:
	expr binary_op expr
	| expr AS type
	| prefix_op expr
	| expr suffix_op
	| list
	| tuple
	| literal
	| SYM;

enum: 'enum' SYM '{' enum_list? '}' ';';

label: SYM ':';

vardef_one: SYM ('=' expr)?;

vardef: ('var' | 'val') vardef_one (',' vardef_one)* ';';

if_stat:
	'if' '(' expr ')' (stat | codeblock) (
		'else' (stat | codeblock)
	)?;

return_stat: RETURN expr? ';';
break_stat: BREAK ';';
continue_stat: CONTINUE ';';
assume_stat: ASSUME expr ';';

for_stat: 'for' '(' ')' (stat | codeblock);

using_module: USING STRING '->' sym ';';
using_type: USING SYM '=' sym ';';
using_namespace: USING sym ';';

codeblock: '{' stat* '}';

type: SYM '*'* '&'?;

// 函数声明
func_decl:
	'fn' SYM // function name
	('[' capture_list? ']')? // capture list
	('(' var_list? ')')? // function arguments
	('->' type)? // return type
	';';

// 函数实现
func_impl:
	'fn' sym // function name
	('[' capture_list? ']')? // capture list
	('(' var_list? ')')? // function arguments
	('->' type)? // return type
	(codeblock | '=' expr ';');

stat:
	vardef
	| func_decl
	| func_impl
	| using_module
	| using_type
	| using_namespace
	| if_stat
	| return_stat
	| break_stat
	| continue_stat
	| assume_stat
	| ';';

prog: stat*;