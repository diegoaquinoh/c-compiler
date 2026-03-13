#include "IRGenVisitor.h"
using namespace std;

string IRGenVisitor::createVariableTmp() {
    string tmpName = "!tmp" + to_string(cptTempVariables++);
    int offset = -4 * cptTempVariables;
    symbolTable[tmpName] = offset;
    return tmpName;
}

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Visit all statements
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }

    // Visit return statement
    this->visit(ctx->return_stmt());


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
        this->IR->cfg->current_bb->add_IRInstr(IRInstr::copy, IntType, {varName, "!reg"});
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    int offset = symbolTable[varName];

    this->visit(ctx->expr());

    this->IR->cfg->current_bb->add_IRInstr(IRInstr::copy, IntType, {varName, "!reg"});

    return 0;
}

antlrcpp::Any IRGenVisitor::visitConst(ifccParser::ConstContext *ctx) 
{
    int val = stoi(ctx->CONST()->getText());
    string varName = IRGenVisitor::createVariableTmp();

    this->IR->cfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, {varName, "!reg"});

    return 0;
}

antlrcpp::Any IRGenVisitor::visitVar(ifccParser::VarContext *ctx)
{
    string varName = ctx->VAR()->getText();

    this->IR->cfg->current_bb->add_IRInstr(IRInstr::copy, IntType, {varName, "!reg"});

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

    string indexTmp = createVariableTmp();
    this->IR->cfg->current_bb->add_IRInstr(IRInstr::copy, IntType, {"!reg", indexTmp});
    
    this->visit(ctx->expr(1));
    
    if (op == "*") {
        this->IR->cfg->current_bb->add_IRInstr(IRInstr::mul, IntType, {"!reg", indexTmp, "!reg"});
    } else {
        this->IR->cfg->current_bb->add_IRInstr(IRInstr::div, IntType, {"!reg", indexTmp, "!reg"});
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    string indexTmp = createVariableTmp();
    this->IR->cfg->current_bb->add_IRInstr(IRInstr::copy, IntType, {"!reg", indexTmp});
    
    this->visit(ctx->expr(1));

    if (op == "+") {
        this->IR->cfg->current_bb->add_IRInstr(IRInstr::add, IntType, {"!reg", indexTmp, "!reg"});
    } else {
        this->IR->cfg->current_bb->add_IRInstr(IRInstr::sub, IntType, {"!reg", indexTmp, "!reg"});
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitNegative(ifccParser::NegativeContext *ctx){
    this->visit(ctx->expr());
    this->IR->cfg->current_bb->add_IRInstr(IRInstr::neg, IntType, {"!reg", "!reg"});
    return 0;
}

antlrcpp::Any IRGenVisitor::visitParens(ifccParser::ParensContext *ctx){
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    this->IR->cfg->current_bb->add_IrInstr(IRInstr::copy, IntType, {"!reg", varName});

    this->visit(ctx->expr(1));
    this->IR->cfg->current_bb->add_IrInstr(IRInstr::band, IntType, {"!reg", varName, varName});

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    this->IR->cfg->current_bb->add_IrInstr(IRInstr::copy, IntType, {"!reg", varName});

    this->visit(ctx->expr(1));
    this->IR->cfg->current_bb->add_IrInstr(IRInstr::bxor, IntType, {"!reg", varName, varName});

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){

    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    this->IR->cfg->current_bb->add_IrInstr(IRInstr::copy, IntType, {"!reg", varName});

    this->visit(ctx->expr(1));
    this->IR->cfg->current_bb->add_IrInstr(IRInstr::bor, IntType, {"!reg", varName, varName});

    return 0;
}