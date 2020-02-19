grammar mu_grammar;

program: block;

block : stmt*;

varInit : ID ('=' expression)?;
varList : varInit (',' varInit)*;

stmt
	: 'break'
	| 'return' expression
	| 'continue'
	| assignment
	| expression
	;

assignment
	: ID trailer* ASSIGN expression
	;

varDeclare
	: 'let' varList
	;

expressionList
	: expression (',' expression)*
	;

expression
	: ('-' | '+' | '~' | '!') expression
	| expression '|' expression
	| expression '^' expression
	| expression '&' expression
	| expression ('<<' | '>>') expression
	| expression ('*' | '/' | '%') expression
	| expression ('+' | '-') expression
	| expression COMP expression
	| expression EQ expression
	| expression '&&' expression
	| expression '||' expression
	| expression 'in' expression
	| expression '..' expression
	| NUMBER
	| ID trailer*
	| STRING trailer*
	| list trailer*
	| 'nil'
	| ID '(' expressionList? ')' trailer*
	| 'if' expression ':' block ('elif' expression ':')* ('else' ':')?
	| 'for' expression 'in' expression ':' block
	| 'fun' ID? '(' varList? ')' ':' block
	| '(' expression ')'
	;

trailer
	: '(' (expressionList)? ')'
	| '[' expression ']'
	| '.' ID
	;

list
	: '[' expressionList* ']'
	;

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
	|	INT ('.' DIGIT+)?
	;

WS : [ \r\n\t\f] -> skip;
COMP : '<' | '>' | '<=' | '>=';
EQ : '!=' | '==';
ASSIGN : '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^=' | '<<=' | '>>=' | '=';

fragment DIGIT : [0-9];
fragment INT
	: [1-9] DIGIT*
	| '0'
	;
fragment LETTER : [a-zA-Z];