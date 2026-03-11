#include "IRGenVisitor.h"
using namespace std;

int IRGenVisitor::createVariableTmp()
{
    this->indexVariables -= 4;
    std::cout << "movl %eax, " << this->indexVariables << "(%rbp)\n";

    return this->indexVariables;
}

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
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

    // On change la valeur du pointeur de pile pour réserver le partie du dessous aux variables et ne pas les écraser
    int stackSize = symbolTable.size() * 4 + 4;
    this->indexVariables = - stackSize ;

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

antlrcpp::Any IRGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    if (ctx->expr()) {
        this->visit(ctx->expr());
        cout << "    movl %eax, " << offset << "(%rbp)\n";
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    this->visit(ctx->expr());
    cout << "    movl %eax, " << offset << "(%rbp)\n";

    return 0;
}

antlrcpp::Any IRGenVisitor::visitConst(ifccParser::ConstContext *ctx) 
{
    int val = stoi(ctx->CONST()->getText());
    cout << "    movl $" << val << ", %eax\n";

    return 0;
}

antlrcpp::Any IRGenVisitor::visitVar(ifccParser::VarContext *ctx)
{
    int offsetSrc = symbolTable[ctx->VAR()->getText()];
    cout << "    movl " << offsetSrc << "(%rbp), %eax\n";

    return 0;
}

antlrcpp::Any IRGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any IRGenVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    auto indexTmp = createVariableTmp();
    
    this->visit(ctx->expr(1));

    if (op == "*") {
        std::cout << "    imull " << indexTmp << "(%rbp), %eax\n";
    } else {
 
        // On met la DROITE (numérateur) dans %ecx
        std::cout << "    movl %eax, %ecx\n";
        
        // On met la GAUCHE (dénominateur) dans %eax
        std::cout << "    movl " << indexTmp << "(%rbp), %eax\n";

        // Extension de signe (Obligatoire pour idivl)
        // Étend le signe de %eax vers %edx pour former le nombre 64 bits %edx:%eax
        std::cout << "    cltd\n";

        // % eax =  (%edx:%eax) / %ecx
        std::cout << "    idivl %ecx\n";
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    int indexTmp = createVariableTmp();
    
    this->visit(ctx->expr(1));

    if (op == "+") {
        std::cout << "    addl " << indexTmp << "(%rbp), %eax\n";
    } else {
        std::cout << "    movl %eax, %ecx\n";      // expr(1) dans %ecx
        std::cout << "    movl " << indexTmp << "(%rbp), %eax\n"; // expr(0) dans %eax
        std::cout << "    subl %ecx, %eax\n";      // %eax = expr(0) - expr(1)
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitNegative(ifccParser::NegativeContext *ctx){
    this->visit(ctx->expr());
    std::cout << "    negl %eax\n";
    return 0;
}

antlrcpp::Any IRGenVisitor::visitParens(ifccParser::ParensContext *ctx){
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    this->visit(ctx->expr(0));
    int indexTmp = createVariableTmp();
    
    this->visit(ctx->expr(1));

    std::cout << "    andl " << indexTmp << "(%rbp), %eax\n";

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    int indexTmp = createVariableTmp();
    
    this->visit(ctx->expr(1));

    std::cout << "    xorl " << indexTmp << "(%rbp), %eax\n";

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){
    this->visit(ctx->expr(0));
    int indexTmp = createVariableTmp();
    
    this->visit(ctx->expr(1));

    std::cout << "    orl " << indexTmp << "(%rbp), %eax\n";

    return 0;
}