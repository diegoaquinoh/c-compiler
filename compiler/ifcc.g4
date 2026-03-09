grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* return_stmt '}' ;

stmt : decl_stmt | affect_stmt ;

decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : VAR ('=' expr)? ;

affect_stmt: VAR '=' expr ';' ;

return_stmt : RETURN expr ';' ;


expr : '-' expr                # negative
     | expr OP=('*'|'/') expr  # multdiv
     | expr OP=('+'|'-') expr  # addsub
     | expr '&' expr           # bitwiseand
     | expr '^' expr           # bitwisexor
     | expr '|' expr           # bitwiseor
     | '(' expr ')'            # parens
     | CONST                   # const
     | VAR                     # var
     ;


RETURN : 'return' ;
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
