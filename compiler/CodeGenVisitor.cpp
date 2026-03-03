#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Compute stack frame size aligned to 16 bytes
    int stackSize = (int)symbolTable.size() * 4;
    stackSize = ((stackSize + 15) / 16) * 16;
    if (stackSize == 0) stackSize = 16;

    #ifdef __APPLE__
    std::cout << "    .globl _main\n";
    std::cout << "_main:\n";
    #else
    std::cout << "    .globl main\n";
    std::cout << "main:\n";
    #endif

    // Prologue
    std::cout << "    pushq %rbp\n";
    std::cout << "    movq %rsp, %rbp\n";
    std::cout << "    subq $" << stackSize << ", %rsp\n";

    // Initialize implicit return value slot at -4(%rbp)
    std::cout << "    movl $0, -4(%rbp)\n";

    // Visit all statements
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }

    // Visit return statement
    this->visit(ctx->return_stmt());

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclVoid(ifccParser::DeclVoidContext *ctx)
{
    // No code needed: variable slot is already reserved on the stack
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclConst(ifccParser::DeclConstContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    int offset = symbolTable[varName];
    std::cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclVar(ifccParser::DeclVarContext *ctx)
{
    // int VAR(0) = VAR(1)
    std::string dst = ctx->VAR(0)->getText();
    std::string src = ctx->VAR(1)->getText();
    int offsetSrc = symbolTable[src];
    int offsetDst = symbolTable[dst];
    std::cout << "    movl " << offsetSrc << "(%rbp), %eax\n";
    std::cout << "    movl %eax, " << offsetDst << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectConst(ifccParser::AffectConstContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    int offset = symbolTable[varName];
    std::cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectVar(ifccParser::AffectVarContext *ctx)
{
    // VAR(0) = VAR(1)
    std::string dst = ctx->VAR(0)->getText();
    std::string src = ctx->VAR(1)->getText();
    int offsetSrc = symbolTable[src];
    int offsetDst = symbolTable[dst];
    std::cout << "    movl " << offsetSrc << "(%rbp), %eax\n";
    std::cout << "    movl %eax, " << offsetDst << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnConst(ifccParser::ReturnConstContext *ctx)
{
    int val = stoi(ctx->CONST()->getText());
    std::cout << "    movl $" << val << ", %eax\n";
    std::cout << "    leave\n";
    std::cout << "    ret\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];
    std::cout << "    movl " << offset << "(%rbp), %eax\n";
    std::cout << "    leave\n";
    std::cout << "    ret\n";
    return 0;
}
