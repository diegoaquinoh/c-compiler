#include "CodeGenVisitor.h"
using namespace std;

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    #ifdef __APPLE__
    cout << "    .globl _main\n";
    cout << "_main:\n";
    #else
    cout << "    .globl main\n";
    cout << "main:\n";
    #endif

    // Prologue
    cout << "    pushq %rbp\n";
    cout << "    movq %rsp, %rbp\n";

    // Initialize implicit return value slot at -4(%rbp)
    cout << "    movl $0, -4(%rbp)\n";

    // Visit all statements
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }

    // Visit return statement
    this->visit(ctx->return_stmt());

    // Epilogue
    cout << "    popq %rbp\n";
    cout << "    retq\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclVoid(ifccParser::DeclVoidContext *ctx)
{
    // No code needed
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclConst(ifccParser::DeclConstContext *ctx)
{
    auto vars = ctx->VAR();
    auto consts = ctx->CONST();
    for (int i = 0; i < vars.size(); i++) {
        int val = stoi(consts[i]->getText());
        int offset = symbolTable[vars[i]->getText()];
        cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclVar(ifccParser::DeclVarContext *ctx)
{
    auto vars = ctx->VAR();
    for (int i = 0; i + 1 < vars.size(); i += 2) {
        int offsetVar1 = symbolTable[vars[i]->getText()];
        int offsetVar2 = symbolTable[vars[i + 1]->getText()];
        cout << "    movl " << offsetVar2 << "(%rbp), %eax\n";
        cout << "    movl %eax, " << offsetVar1 << "(%rbp)\n";
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectConst(ifccParser::AffectConstContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    int offset = symbolTable[varName];
    cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectVar(ifccParser::AffectVarContext *ctx)
{
    string var1 = ctx->VAR(0)->getText();
    string var2 = ctx->VAR(1)->getText();
    int offsetVar1 = symbolTable[var1];
    int offsetVar2 = symbolTable[var2];
    cout << "    movl " << offsetVar2 << "(%rbp), %eax\n";
    cout << "    movl %eax, " << offsetVar1 << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnConst(ifccParser::ReturnConstContext *ctx)
{
    int val = stoi(ctx->CONST()->getText());
    cout << "    movl $" << val << ", %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];
    cout << "    movl " << offset << "(%rbp), %eax\n";
    return 0;
}
