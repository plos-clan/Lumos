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
IS: 'is';
VAR: 'var';
VAL: 'val';
IMV: 'imv';
LIT: 'lit';
FIN: 'fin';
LET: 'let';
OBJ: 'obj';
MUT: 'mut';
IMM: 'imm';
RO: 'ro';
WO: 'wo';
RW: 'rw';
RX: 'rx';
IF: 'if';
ELSE: 'else';
ELIF: 'elif';
THEN: 'then';
SWITCH: 'switch';
MATCH: 'match';
FOR: 'for';
WHILE: 'while';
DO: 'do';
GOTO: 'goto';
NAMESPACE: 'namespace';
STRUCT: 'struct';
CLASS: 'class';
ENUM: 'enum';
UNION: 'union';
IMPL: 'impl';
TEMPLATE: 'template';
BY: 'by';
VARIANT: 'variant';
LEAVE: 'leave';
MEASURE: 'measure';
YIELDS: 'yields';
WITH: 'with';
PERMISSION: 'permission';
AKA: 'aka';
DEF: 'def';
FUN: 'fun';
ACT: 'act';
ASYNC: 'async';
AWAIT: 'await';
ONCE: 'once' [!?]?;
UNSAFE: 'unsafe';

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
OP_BAND: '&';
OP_BOR: '|';
OP_BXOR: '^';
OP_SHL: '<<';
OP_SHR: '>>';
OP_USHL: '<<<';
OP_USHR: '>>>';
OP_ROL: '<<>';
OP_ROR: '>><';
OP_ADD_ASSIGN: '+=';
OP_SUB_ASSIGN: '-=';
OP_MUL_ASSIGN: '*=';
OP_DIV_ASSIGN: '/=';
OP_MOD_ASSIGN: '%=';
OP_XOR_ASSIGN: '^=';
OP_AND_ASSIGN: '&=';
OP_OR_ASSIGN: '|=';
OP_SHL_ASSIGN: '<<=';
OP_SHR_ASSIGN: '>>=';
OP_RANGE: '..';
OP_ARROW: '->';
OP_DCOLON: '::';
OP_BACKSLASH: '\\';

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
	| OP_OR
	| OP_BAND
	| OP_BOR
	| OP_BXOR
	| OP_SHL
	| OP_SHR
	| OP_USHL
	| OP_USHR
	| OP_ROL
	| OP_ROR
	| OP_RANGE;

assign_op:
	'='
	| OP_ADD_ASSIGN
	| OP_SUB_ASSIGN
	| OP_MUL_ASSIGN
	| OP_DIV_ASSIGN
	| OP_MOD_ASSIGN
	| OP_XOR_ASSIGN
	| OP_AND_ASSIGN
	| OP_OR_ASSIGN
	| OP_SHL_ASSIGN
	| OP_SHR_ASSIGN;

// 1, 2, 3
val_list: expr (',' expr)* ','?;
// int a where a > 0 = 1, int b, int c
var_decl: type SYM (WHERE expr)? ('=' expr)?;
var_list: var_decl (',' var_decl)* ','?;
// .xxx = 1, 2, .zzz = 3
elm_list: ('.' SYM '=')? expr (',' ('.' SYM '=')? expr)* ','?;
// xxx = 1, yyy, zzz
enum_list: SYM ('=' expr)? (',' SYM ('=' expr)?)* ','?;

capture_list: (OP_BAND? expr | '*') (',' (OP_BAND? expr | '*'))* ','?;

map: '{' val_list? '}';
struct_init: type? '{' elm_list? '}';
list: '[' val_list? ']';
tuple: '(' val_list? ')';

expr:
	expr binary_op expr
	| expr assign_op expr
	| expr AS type
	| expr IS type
	| prefix_op expr
	| expr suffix_op
	| list
	| tuple
	| map
	| struct_init
	| literal
	| sym
	| lambda;

lambda:
	(DEF | FUN | ACT) ('[' (permission_list | capture_list)? ']')? '(' var_list? ')' (
		'->' type
	)? (YIELDS '[' permission_list ']')? (codeblock | '=' expr);

enum:
	ENUM SYM (BY type)? (AS (SYM | 'table' | 'container'))? '{' enum_list? '}' ';';

label: SYM ':';

vardef_one: SYM (WHERE expr)? ('=' expr)?;

vardef: (VAR | VAL | IMV | LIT | FIN | LET | OBJ) vardef_one (
		',' vardef_one
	)* ';';

if_stat:
	IF '(' val_list ')' (stat | codeblock) (
		ELIF '(' val_list ')' (stat | codeblock)
	)* (ELSE (stat | codeblock))?;

post_if_stat: stat IF expr ';';
then_stat: expr THEN stat;

switch_case: (expr | ATTR) ':' (stat | codeblock);
switch_stat: SWITCH '(' expr ')' '{' switch_case* '}';

match_case: (expr | ATTR) ':' (stat | codeblock);
match_stat: MATCH ('(' expr ')')? '{' match_case* '}';

return_stat: RETURN expr? ';';
break_stat: BREAK ';';
continue_stat: CONTINUE ';';
assume_stat: ASSUME expr ';';
leave_stat: LEAVE ';';
goto_stat: GOTO SYM ';';

while_stat: WHILE '(' expr ')' (stat | codeblock);
do_while_stat: DO (stat | codeblock) WHILE '(' expr ')' ';';
for_stat:
	FOR '(' (vardef | expr? ';') expr? ';' expr? ')' (
		stat
		| codeblock
	);

using_module: USING STRING (OP_ARROW sym)? ';';
using_type: USING SYM '=' type ';';
using_namespace: USING sym ';';
using_attr: USING ATTR '=' ATTR+ ';';

codeblock: '{' stat* '}';

type_mod: MUT | IMM | RO | WO | RW | RX;
type:
	type_mod? (
		sym
		| '[' type ']' '?'?
		| OP_BAND type
		| '[' expr ']' type
		| '[]' type
	) (OP_LT type_list OP_GT)?;
type_list: type (',' type)*;

permission: (sym | '%');
permission_list: permission (',' permission)*;

// 函数声明
func_decl:
	ATTR* (DEF | UNSAFE? FUN | ONCE? ACT) (
		OP_BACKSLASH (SYM | OP)
	)? SYM // function name
	(OP_LT type_list OP_GT)? // template params
	('[' (permission_list | capture_list)? ']')? // permissions or capture list
	('(' var_list? ')')? // function arguments
	('->' type)? // return type
	(YIELDS '[' permission_list ']')? // yielded permissions
	';';

// 函数实现
func_impl:
	ATTR* (DEF | UNSAFE? FUN | ONCE? ACT) (
		OP_BACKSLASH (SYM | OP)
	)? sym // function name
	(OP_LT type_list OP_GT)? // template params
	('[' (permission_list | capture_list)? ']')? // permissions or capture list
	('(' var_list? ')')? // function arguments
	('->' type)? // return type
	(YIELDS '[' permission_list ']')? // yielded permissions
	(codeblock | '=' expr ';');

class_def:
	ATTR* (CLASS | STRUCT | UNION) SYM (OP_LT type_list OP_GT)? (
		':' type_list
	)? ('{' (stat | access_mod)* '}' | ';');

access_mod: ATTR ':';

template_def: TEMPLATE SYM '(' SYM (',' SYM)* ')' codeblock;

impl_def:
	IMPL (OP_LT type_list OP_GT)? type (FOR type)? codeblock;

stat:
	vardef
	| func_decl
	| func_impl
	| class_def
	| enum
	| using_module
	| using_type
	| using_namespace
	| using_attr
	| if_stat
	| post_if_stat
	| then_stat
	| switch_stat
	| match_stat
	| while_stat
	| do_while_stat
	| for_stat
	| return_stat
	| break_stat
	| continue_stat
	| leave_stat
	| assume_stat
	| goto_stat
	| template_def
	| impl_def
	| expr ';'
	| label
	| ';';

prog: stat*;