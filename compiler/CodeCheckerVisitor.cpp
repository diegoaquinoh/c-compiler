#include "CodeCheckerVisitor.h"

// TO VERIFY:
// Une variable utilisée dans une expression a été déclarée ; GOOD
// Une variable n’est pas déclarée plusieurs fois ; GOOD
// Une variable déclarée est utilisée au moins une fois. GOOD

antlrcpp::Any CodeCheckerVisitor::visitProg(ifccParser::ProgContext *ctx) {
    visitChildren(ctx);
    
    for (const auto& var : symbolTable) {
        if (var.second == 0) {
            std::cerr << "Warning: variable '" << var.first << "' declared but never used" << std::endl;
        }
    }
    
    return 0;
}

antlrcpp::Any CodeCheckerVisitor::visitReturn_const_stmt(ifccParser::Return_const_stmtContext *ctx){
    int retval = stoi(ctx->CONST()->getText());

    return 0;
}
antlrcpp::Any CodeCheckerVisitor::visitReturn_var_stmt(ifccParser::Return_var_stmtContext *ctx){
    std::string varName = ctx->VARNAME()->getText();

    if (symbolTable.find(varName) == symbolTable.end()) {
        std::cerr << "Error: undefined variable '" << varName << "'" << std::endl;
        exit(1);
    }
    
    symbolTable[varName] = 1;

    return 0;
}
antlrcpp::Any CodeCheckerVisitor::visitInit_stmt_const(ifccParser::Init_stmt_constContext *ctx){
    std::string varName = ctx->VARNAME()->getText();

    if (symbolTable.find(varName) != symbolTable.end()) {
        std::cerr << "Error: variable '" << varName << "' already declared" << std::endl;
        exit(1);
    }
    
    symbolTable[varName] = 0;

    return 0;
}

antlrcpp::Any CodeCheckerVisitor::visitInit_stmt_no_const(ifccParser::Init_stmt_no_constContext *ctx){
    std::string varName = ctx->VARNAME()->getText();
    
    if (symbolTable.find(varName) != symbolTable.end()) {
        std::cerr << "Error: variable '" << varName << "' already declared" << std::endl;
        exit(1);
    }
    
    symbolTable[varName] = 0;
    
    return 0;
}

antlrcpp::Any CodeCheckerVisitor::visitAssign_stmt_var(ifccParser::Assign_stmt_varContext *ctx){
    std::string varName = ctx->VARNAME(0)->getText();
    std::string varNameValue = ctx->VARNAME(1)->getText();

    if (symbolTable.find(varName) == symbolTable.end()) {
        std::cerr << "Error: undefined variable '" << varName << "'" << std::endl;
        exit(1);
    }

    if (symbolTable.find(varNameValue) == symbolTable.end()) {
        std::cerr << "Error: undefined variable '" << varNameValue << "'" << std::endl;
        exit(1);
    }
    
    symbolTable[varNameValue] = 1;

    return 0;
}

antlrcpp::Any CodeCheckerVisitor::visitAssign_stmt_const(ifccParser::Assign_stmt_constContext *ctx){
    std::string varName = ctx->VARNAME()->getText();
    
    if (symbolTable.find(varName) == symbolTable.end()) {
        std::cerr << "Error: undefined variable '" << varName << "'" << std::endl;
        exit(1);
    }
    
    return 0;
}
