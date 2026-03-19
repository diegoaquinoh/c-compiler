#include "IRGenVisitor.h"
using namespace std;

string reg = "!reg";

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
        vector<string> v = {varName, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();

    this->visit(ctx->expr());

    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitConst(ifccParser::ConstContext *ctx) 
{
    int val = stoi(ctx->CONST()->getText());

    vector<string> v = {reg, to_string(val)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, v);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitVar(ifccParser::VarContext *ctx)
{
    string varName = ctx->VAR()->getText();

    vector<string> v = {reg, varName};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

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
    vector<string> v = {indexTmp, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
    
    this->visit(ctx->expr(1));
    
    if (op == "*") {
        vector<string> v2 = {reg, indexTmp, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::mul, IntType, v2);
    } else {
        vector<string> v3 = {reg, indexTmp, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::div, IntType, v3);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    string indexTmp = createVariableTmp();
    vector<string> v = {indexTmp, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
    
    this->visit(ctx->expr(1));

    if (op == "+") {
        vector<string> v2 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::add, IntType, v2);
    } else {
        vector<string> v3 = {reg, indexTmp, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::sub, IntType, v3);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitNegative(ifccParser::NegativeContext *ctx){
    this->visit(ctx->expr());
    vector<string> v = {reg, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::neg, IntType, v);
    return 0;
}

antlrcpp::Any IRGenVisitor::visitParens(ifccParser::ParensContext *ctx){
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    this->visit(ctx->expr(1));
    vector<string> v2 = {reg, varName, varName};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::band, IntType, v2);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    this->visit(ctx->expr(1));
    vector<string> v2 = {reg, varName, varName};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::bxor, IntType, v2);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){

    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    this->visit(ctx->expr(1));
    vector<string> v2 = {reg, varName, varName};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::bor, IntType, v2);

    return 0;
}