grammar mu_grammar;

program: block;

varInit : ID ('=' test)?;
varList : varInit (',' varInit)*;

block : stmt*;
body : stmt | '{' block '}';

stmt
	: ';'
	| 'break' ';'
	| 'continue' ';'
	| 'return' test? ';'
	| 'let' varList ';'
	| 'if' test body ('elif' body)* ('else' body)?
	| 'for' ID 'in' (list | range) body?
	| 'while' test body?
	| 'fun' ID '(' varList ')' body // FUNCTION
	| test ';'
	;

range
	: test '..' test
	;

list
	: '[]'
	| '[' arglist ']'
	;

test : ternary (ASSIGN test)?;

ternary : or ('?' test ':' test)?;

or : and ('||' and)*;

and : not ('&&' not)*;

not : '!' comparisonEq | comparisonEq;

comparisonEq : comparison (EQ comparison)*;

comparison : expr (COMP expr)*;

expr : bitXor ('|' bitXor)*;

bitXor : bitAnd ('^' bitAnd)*;

bitAnd : shift ('&' shift)*;

shift : arith (('<<'|'>>') arith)*;

arith : term (('+'|'-') term)*;

term : factor (('*'|'/'|'%') factor)*;

factor : ('-'|'+'|'~') factor | trail;

trail : atom trailer*;

atom : ID | NUMBER | STRING;

trailer
	: '(' (arglist)? ')'
	| '[' test ']'
	| '.' ID
	;

arglist : test (',' test)*;

STRING
	:	'"' ( '\\"' | . )*? '"'
	;

ID
	:	[a-zA-Z_][a-zA-Z0-9]*
	;

HEX
	:	('0x'|'0X')? [0-9a-zA-Z]+
	;

NUMBER
	:	HEX
	|	DIGIT+
	|	DIGIT* '.' DIGIT+
	;

WS : [ \r\n\t\f] -> skip;
COMP : '<' | '>' | '<=' | '>=';
EQ : '!=' | '==';
ASSIGN : '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^=' | '<<=' | '>>=' | '=';

fragment DIGIT : [0-9];
fragment LETTER : [a-zA-Z];