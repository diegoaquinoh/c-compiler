grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' statement+ '}' ;

statement : return_stmt | init_stmt | assign_stmt ;

return_stmt : RETURN CONST ';' # return_const_stmt
            | RETURN VARNAME ';' # return_var_stmt ;

init_stmt : 'int' VARNAME '=' CONST ';' # init_stmt_const 
          | 'int' VARNAME ';' # init_stmt_no_const ;

assign_stmt : VARNAME '=' VARNAME ';' # assign_stmt_var
            | VARNAME '=' CONST ';' # assign_stmt_const ;

RETURN : 'return' ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
VARNAME : [a-zA-Z] ;

