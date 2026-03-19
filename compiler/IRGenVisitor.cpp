#include "IRGenVisitor.h"
using namespace std;

string reg = "!reg";

string IRGenVisitor::createVariableTmp() {
    return "!tmp" + to_string(cptTempVariables++);
}

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Prologue:

    // Visit all statements
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }

    // Visit return statement
    this->visit(ctx->return_stmt());

    // Epilogue:
    

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

antlrcpp::Any IRGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{

    CFG *cfg = this->ir.currentCfg;
    BasicBlock *trueBB = new BasicBlock(cfg, cfg->new_BB_name() + "_true");
    BasicBlock *nextBB = this->ir.currentCfg->current_bb->exit_true;
    BasicBlock* currentBB = cfg->current_bb;


    BasicBlock *elseBB = nullptr;
    if (ctx->else_stmt()) {
        elseBB = new BasicBlock(cfg, cfg->new_BB_name() + "_else");
    }

    currentBB->exit_true = trueBB;
    currentBB->exit_false = (elseBB != nullptr) ? elseBB : nextBB;

    this->visit(ctx->expr());

    cfg->add_bb(trueBB);

    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
        this->ir.currentCfg->current_bb->exit_true = nextBB;
    }

    if (elseBB) {
        cfg->add_bb(elseBB);
        this->visit(ctx->else_stmt());
        this->ir.currentCfg->current_bb->exit_true = nextBB;
    }

    cfg->current_bb = nextBB;

    return 0;
}

antlrcpp::Any IRGenVisitor::visitElse_stmt(ifccParser::Else_stmtContext *ctx)
{
    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    return 0;
}


antlrcpp::Any IRGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    this->visit(ctx->expr());
    vector<string> v = {reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::rtrn, IntType, v);

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
    } else if (op == "/") {
        vector<string> v3 = {reg, indexTmp, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::div, IntType, v3);
    } else if (op == "%") {
        vector<string> v4 = {reg, indexTmp, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::mod, IntType, v4);
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
    vector<string> v2 = {reg, varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::band, IntType, v2);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    this->visit(ctx->expr(1));
    vector<string> v2 = {reg, varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::bxor, IntType, v2);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){

    this->visit(ctx->expr(0));
    string varName = createVariableTmp();
    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    this->visit(ctx->expr(1));
    vector<string> v2 = {reg, varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::bor, IntType, v2);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitRelational(ifccParser::RelationalContext *ctx){
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    string indexTmp = createVariableTmp();
    vector<string> v = {indexTmp, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
    
    this->visit(ctx->expr(1));

    if (op == "<") {
        vector<string> v2 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_lt, IntType, v2);
    } else if (op == "<=") {
        vector<string> v3 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_le, IntType, v3);
    } else if (op == ">") {
        vector<string> v4 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_gt, IntType, v4);
    } else if (op == ">=") {
        vector<string> v5 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_ge, IntType, v5);
    }

    return 0;

}

antlrcpp::Any IRGenVisitor::visitEquality(ifccParser::EqualityContext *ctx){
    auto op = ctx->OP->getText();
    this->visit(ctx->expr(0));

    string indexTmp = createVariableTmp();
    vector<string> v = {indexTmp, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
    
    this->visit(ctx->expr(1));

    if (op == "==") {
        vector<string> v2 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_eq, IntType, v2);
    } else if (op == "!=") {
        vector<string> v3 = {string(reg), indexTmp, string(reg)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_ne, IntType, v3);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitLogicalnot(ifccParser::LogicalnotContext *ctx) {
    this->visit(ctx->expr());
    vector<string> v = {reg, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::lnot, IntType, v);
    
    return 0;
}