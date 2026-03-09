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

    /************* 
    ***Prologue***
    **************/
    cout << "    pushq %rbp\n";
    cout << "    movq %rsp, %rbp\n";

    // Allocation of rsp
    int stackBytes = symbolTable.size() * 4;
    int stackSize = ((stackBytes + 15) / 16) * 16;
    if (hasFuncCall) {
        cout << "    subq $" << stackSize << ", %rsp\n";
    }

    /******************
    Visit all statements
    *******************/
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }

    // Visit return statement
    this->visit(ctx->return_stmt());

    /*********
    *Epilogue* 
    **********/
    if (hasFuncCall) {
        cout << "    addq $" << stackSize << ", %rsp\n";
    }
    if (!hasReturn) {
        cout << "    movl $0, %eax\n";
    }
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
    string varName = ctx->IDENT()->getText();
    int offset = symbolTable[varName];

    if (ctx->expr()) {
        if (ctx->expr()->CONST()) {
            int val = stoi(ctx->expr()->CONST()->getText());
            cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
        } else {
            int offsetSrc = symbolTable[ctx->expr()->IDENT()->getText()];
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

antlrcpp::Any CodeGenVisitor::visitExpr_stmt(ifccParser::Expr_stmtContext *ctx)
{
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx)
{
    string funcName = ctx->IDENT()->getText();
    vector<string> args = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
    for (int i = 0; i < 6; i++) {
        if (ctx->expr(i)) {
            if (ctx->expr(i)->CONST()) {
                int val = stoi(ctx->expr(i)->CONST()->getText());
                cout << "    movl $" << val << ", " << args[i] << "\n";
            } else {
                int offsetSrc = symbolTable[ctx->expr(i)->VAR()->getText()];
                cout << "    movl " << offsetSrc << "(%rbp), " << args[i] << "\n";
            }
        }
    }

    #ifdef __APPLE__
    cout << "    callq _" << funcName << "\n";
    #else
    cout << "    callq " << funcName << "\n";
    #endif


    
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
