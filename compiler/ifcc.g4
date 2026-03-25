grammar ifcc;

axiom : prog EOF ;

prog : func_def+ ;

func_def : RET_TYPE '(' param_list? ')' block ;

param_list : param (',' param)* ;

param : TYPE VAR ;

stmt : decl_stmt | expr ';' | if_stmt | return_stmt;

decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : VAR ('=' expr)? ;

if_stmt: 'if' '(' expr ')' block (else_stmt)? ;
else_stmt: 'else' block ;

block : '{' stmt* '}' ;

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
TYPE : 'int'
RET_TYPE: 'int' | 'void';
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
