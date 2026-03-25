grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* '}' ;

stmt : decl_stmt | expr ';' | if_stmt | return_stmt;

decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : VAR ('=' expr)? ;

affect_stmt: VAR '=' expr ';' ;

if_stmt: 'if' '(' expr ')' '{' stmt* '}' (else_stmt)? ;
else_stmt: 'else' '{' stmt* '}' ;

return_stmt : RETURN expr ';' ;

expr : VAR '(' (expr (',' expr)*)? ')'       # funcCall
     | '(' expr ')'                          # parens
     | '-' expr                              # negative
     | '!' expr                              # logicalnot
     | expr OP=('*'|'/'|'%') expr            # multdiv
     | expr OP=('+'|'-') expr                # addsub
     | expr OP=('<'|'<='|'>'|'>=') expr      # relational
     | expr OP=('=='|'!=') expr              # equality
     | expr '&' expr                         # bitwiseand
     | expr '^' expr                         # bitwisexor
     | expr '|' expr                         # bitwiseor
     | VAR '=' expr                          # affectStmt
     | CONST                                 # const
     | CHAR_CONST                            # const
     | VAR                                   # var
     ;

RETURN : 'return' ;
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
CHAR_CONST : '\'' ( '\\' . | ~['\\] ) '\'' ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
