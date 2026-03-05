#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    #ifdef __APPLE__
    std::cout<< ".globl _main\n" ;
    std::cout<< "_main:\n" ;
    #else
    std::cout<< ".globl main\n" ;
    std::cout<< "main:\n" ;
    #endif

    std::cout << "    pushq %rbp\n";
    std::cout << "    movq %rsp, %rbp\n";

    visitChildren(ctx);

    std::cout << "    popq %rbp\n";
    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_const_stmt(ifccParser::Return_const_stmtContext *ctx){
    int retval = stoi(ctx->CONST()->getText());
    std::cout << "    movl $"<<retval<<", %eax\n" ;
    return 0;
}
antlrcpp::Any CodeGenVisitor::visitReturn_var_stmt(ifccParser::Return_var_stmtContext *ctx){
    std::string varName = ctx->VARNAME()->getText();
    
    int offset = symbolTable[varName];
    std::cout << "    movl " << offset << "(%rbp), %eax\n";
    return 0;
}
antlrcpp::Any CodeGenVisitor::visitInit_stmt_const(ifccParser::Init_stmt_constContext *ctx){
    std::string varName = ctx->VARNAME()->getText();
    int value = stoi(ctx->CONST()->getText());
    
    int offset = nextOffset;
    symbolTable[varName] = offset;
    nextOffset -= 4;
    
    std::cout << "    movl $" << value << ", " << offset << "(%rbp)\n";
    
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitInit_stmt_no_const(ifccParser::Init_stmt_no_constContext *ctx){
    std::string varName = ctx->VARNAME()->getText();
    
    int offset = nextOffset;
    symbolTable[varName] = offset;
    nextOffset -= 4;
    
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssign_stmt_var(ifccParser::Assign_stmt_varContext *ctx){
    std::string varName = ctx->VARNAME(0)->getText();
    std::string varNameValue = ctx->VARNAME(1)->getText();

    int valueAdresse = symbolTable[varNameValue];

    int offset = nextOffset;
    symbolTable[varName] = offset;
    nextOffset -= 4;

    std::cout << "    movl " << valueAdresse << "(%rbp), %eax\n";
    std::cout << "    movl %eax, " << offset << "(%rbp)\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssign_stmt_const(ifccParser::Assign_stmt_constContext *ctx){
    std::string varName = ctx->VARNAME()->getText();
    int value = stoi(ctx->CONST()->getText());
    
    int offset = symbolTable[varName];
    
    std::cout << "    movl $" << value << ", " << offset << "(%rbp)\n";
    
    return 0;
}
