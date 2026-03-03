grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* return_stmt '}' ;

stmt : 'int' VAR ';'                #declVoid
     | 'int' VAR '=' CONST ';'     #declConst
     | 'int' VAR '=' VAR ';'       #declVar
     | VAR '=' CONST ';'           #affectConst
     | VAR '=' VAR ';'             #affectVar
     ;

return_stmt : RETURN CONST ';'     #returnConst
            | RETURN VAR ';'       #returnVar
            ;

RETURN : 'return' ;
VAR : [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN) ;
