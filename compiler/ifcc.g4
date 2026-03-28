grammar ifcc;

axiom : prog EOF ;

prog : func_def+ ;

func_def : TYPE VAR '(' param_list? ')' block ;

param_list : TYPE VAR (',' TYPE VAR)* ;

stmt : decl_stmt
     | expr ';'
     | if_stmt
     | switch_stmt
     | break_stmt
     | while_stmt
     | return_stmt
     ;

decl_stmt : TYPE decl_item (',' decl_item)* ';' ;
decl_item : VAR ('=' expr)? ;

if_stmt: 'if' '(' expr ')' block (else_stmt)? ;
else_stmt: 'else' block ;

block : '{' stmt* '}' ;

while_stmt: 'while' '(' expr ')' '{' stmt* '}' ;
switch_stmt : 'switch' '(' expr ')' '{' switch_clause* '}' ;
switch_clause : case_label stmt* | default_label stmt* ;
case_label : 'case' case_value ':' ;
default_label : 'default' ':' ;
case_value : '-'? CONST ;

break_stmt : 'break' ';' ;

return_stmt : RETURN expr? ';' ;

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
     | DOUBLE_CONST                          # const
     | CONST                                 # const
     | CHAR_CONST                            # const
     | VAR                                   # var
     ;

RETURN : 'return' ;
TYPE : 'int' | 'void' | 'double' ;
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
DOUBLE_CONST : [0-9]+ '.' [0-9]+ ;
CONST : [0-9]+ ;
CHAR_CONST : '\'' ( '\\' . | ~['\\] ) '\'' ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
