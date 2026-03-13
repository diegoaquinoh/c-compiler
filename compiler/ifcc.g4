grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* return_stmt '}' ;

stmt : 'int' decl_item (',' decl_item)* ';'      # declStmt
     | VAR '=' expr ';'                         # affectStmt
     | VAR '(' (expr (',' expr)*)? ')' ';'      # callStmt
     ;

decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : VAR ('=' expr)? ;

affect_stmt: VAR '=' expr ';' ;

return_stmt : RETURN expr ';' ;


expr : VAR '(' (expr (',' expr)*)? ')' # funcCall
     | '(' expr ')'            # parens
     |'-' expr                 # negative
     |'!' expr                 # logicalnot
     | expr OP=('*'|'/'|'%') expr  # multdiv
     | expr OP=('+'|'-') expr  # addsub
     | expr '&' expr           # bitwiseand
     | expr '^' expr           # bitwisexor
     | expr '|' expr           # bitwiseor
     | CONST                   # const
     | VAR                     # var
     ;


RETURN : 'return' ;
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
