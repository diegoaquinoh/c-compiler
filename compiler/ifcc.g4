grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* '}' ;

stmt : decl_stmt
     | expr ';'
     | if_stmt
     | switch_stmt
     | break_stmt
     | return_stmt
     ;

decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : VAR ('=' expr)? ;

if_stmt: 'if' '(' expr ')' '{' stmt* '}' (else_stmt)? ;
else_stmt: 'else' '{' stmt* '}' ;

switch_stmt : 'switch' '(' expr ')' '{' switch_clause* '}' ;
switch_clause : case_label stmt* | default_label stmt* ;
case_label : 'case' case_value ':' ;
default_label : 'default' ':' ;
case_value : '-'? CONST ;

break_stmt : 'break' ';' ;

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
     | CONST                                 # const
     | VAR                                   # var
     | VAR '=' expr                          # affectStmt
     ;

RETURN : 'return' ;
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
