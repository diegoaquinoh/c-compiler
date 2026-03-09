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
    cout << "    subq $256, %rsp\n"; // Allocate stack frame (enough for 64 int slots)

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
    string varName = ctx->IDENT()->getText();
    int offset = symbolTable[varName];

    if (ctx->expr()->CONST()) {
        int val = stoi(ctx->expr()->CONST()->getText());
        cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
    } else {
        int offsetSrc = symbolTable[ctx->expr()->IDENT()->getText()];
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
                int offsetSrc = symbolTable[ctx->expr(i)->IDENT()->getText()];
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
    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    std::cout << "    pushq %rax\n";
    
    this->visit(ctx->expr(1));

    if (op == "*") {
        std::cout << "    imull (%rsp), %eax\n";

        std::cout << "    addq $8, %rsp\n";
    } else {
 
        // On met la DROITE (numérateur) dans %ecx
        std::cout << "    movl %eax, %ecx\n";
        
        // On récupère la GAUCHE (dénominateur) dans %eax
        std::cout << "    popq %rax\n";

        // Extension de signe (Obligatoire pour idivl)
        // Étend le signe de %eax vers %edx pour former le nombre 64 bits %edx:%eax
        std::cout << "    cltd\n";

        // % eax =  (%edx:%eax) / %ecx
        std::cout << "    idivl %ecx\n";
    }

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    std::cout << "    pushq %rax\n";
    
    this->visit(ctx->expr(1));

    if (op == "+") {
        std::cout << "    addl (%rsp), %eax\n";
    } else {
        std::cout << "    subl (%rsp), %eax\n";
    }

    std::cout << "    addq $8, %rsp\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitNegative(ifccParser::NegativeContext *ctx){
    this->visit(ctx->expr());
    std::cout << "    negl %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    this->visit(ctx->expr(0));
    std::cout << "    pushq %rax\n";
    
    this->visit(ctx->expr(1));

    std::cout << "    andl (%rsp), %eax\n";
    std::cout << "    addq $8, %rsp\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    std::cout << "    pushq %rax\n";
    
    this->visit(ctx->expr(1));

    std::cout << "    xorl (%rsp), %eax\n";
    std::cout << "    addq $8, %rsp\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){
    this->visit(ctx->expr(0));
    std::cout << "    pushq %rax\n";
    
    this->visit(ctx->expr(1));

    std::cout << "    orl (%rsp), %eax\n";
    std::cout << "    addq $8, %rsp\n";

    return 0;
}