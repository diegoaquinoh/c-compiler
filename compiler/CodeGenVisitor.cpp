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
        this->visit(ctx->expr());
        cout << "    movl %eax, " << offset << "(%rbp)\n";
    }

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    this->visit(ctx->expr());
    cout << "    movl %eax, " << offset << "(%rbp)\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitConst(ifccParser::ConstContext *ctx) 
{
    int val = stoi(ctx->CONST()->getText());
    cout << "    movl $" << val << ", %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitVar(ifccParser::VarContext *ctx)
{
    int offsetSrc = symbolTable[ctx->VAR()->getText()];
    cout << "    movl " << offsetSrc << "(%rbp), %eax\n";

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

antlrcpp::Any CodeGenVisitor::visitParens(ifccParser::ParensContext *ctx){
    this->visit(ctx->expr());
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