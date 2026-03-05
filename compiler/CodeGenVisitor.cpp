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

antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    if (ctx->expr()) {
        if (ctx->expr()->CONST()) {
            int val = stoi(ctx->expr()->CONST()->getText());
            cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
        } else {
            int offsetSrc = symbolTable[ctx->expr()->VAR()->getText()];
            cout << "    movl " << offsetSrc << "(%rbp), %eax\n";
            cout << "    movl %eax, " << offset << "(%rbp)\n";
        }
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    if (ctx->expr()->CONST()) {
        int val = stoi(ctx->expr()->CONST()->getText());
        cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    } else {
        int offsetSrc = symbolTable[ctx->expr()->VAR()->getText()];
        cout << "    movl " << offsetSrc << "(%rbp), %eax\n";
        cout << "    movl %eax, " << offset << "(%rbp)\n";
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    if (ctx->expr()->CONST()) {
        int val = stoi(ctx->expr()->CONST()->getText());
        cout << "    movl $" << val << ", %eax\n";
    } else {
        int offset = symbolTable[ctx->expr()->VAR()->getText()];
        cout << "    movl " << offset << "(%rbp), %eax\n";
    }
    return 0;
}
