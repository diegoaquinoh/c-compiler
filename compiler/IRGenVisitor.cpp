#include "IRGenVisitor.h"
using namespace std;

string reg = "!reg";

string IRGenVisitor::createVariableTmp() {
    return "!tmp" + to_string(cptTempVariables++);
}

// --- Scope management for variable renaming ---

void IRGenVisitor::enterScope() {
    scopeStack.push_back({});
}

void IRGenVisitor::exitScope() {
    scopeStack.pop_back();
}

string IRGenVisitor::declareScoped(const string &name) {
    // Check if name already exists in any outer scope
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--) {
        if (scopeStack[i].count(name)) {
            // Shadowing: create a unique name
            string uniqueName = name + "@" + to_string(scopeCounter++);
            scopeStack.back()[name] = uniqueName;
            return uniqueName;
        }
    }
    // First declaration: use original name
    scopeStack.back()[name] = name;
    return name;
}

string IRGenVisitor::scopedName(const string &name) {
    // Search from innermost scope outward
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--) {
        if (scopeStack[i].count(name)) {
            return scopeStack[i][name];
        }
    }
    return name; // fallback (shouldn't happen if semantic analysis passed)
}

// --- Visitors ---

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (auto *func : ctx->func_def()) {
        this->visit(func);
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitFunc_def(ifccParser::Func_defContext *ctx)
{
    string funcName = ctx->VAR()->getText();

    // Create a new CFG for this function
    CFG* cfg = new CFG(&this->ir);
    cfg->functionName = funcName;

    // Reset scope state
    scopeStack.clear();
    scopeCounter = 0;
    enterScope(); // function-level scope

    // Register parameter names
    if (ctx->param_list()) {
        for (auto *param : ctx->param_list()->VAR()) {
            string paramName = param->getText();
            string irName = declareScoped(paramName);
            cfg->paramNames.push_back(irName);
            cfg->add_to_symbol_table(irName, IntType);
        }
    }

    // Set up basic blocks: prologue -> body -> epilogue
    BasicBlock* prologueBB = new BasicBlock(cfg, funcName + "_prologue");
    BasicBlock* body = new BasicBlock(cfg, funcName + "_body");
    BasicBlock* epilogue = new BasicBlock(cfg, funcName + "_epilogue");

    prologueBB->exit_true = body;
    body->exit_true = epilogue;

    cfg->add_bb(prologueBB);
    cfg->add_bb(body);
    cfg->add_bb(epilogue);
    cfg->current_bb = body;

    // Store in IR
    this->ir.cfgsMap[funcName] = cfg;
    this->ir.currentCfg = cfg;

    // Reset temp variable counter per function
    cptTempVariables = 0;

    // Visit the function body
    this->visit(ctx->block());

    // Implicit return 0 for main (or any int function without explicit return)
    if (!cfg->current_bb->has_return) {
        string retType = ctx->TYPE()->getText();
        if (retType == "int") {
            vector<string> loadZero = {reg, "0"};
            cfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, loadZero);

            vector<string> retZero = {reg};
            cfg->current_bb->add_IRInstr(IRInstr::rtrn, IntType, retZero);
            cfg->current_bb->has_return = true;
        }
        cfg->current_bb->exit_true = nullptr;
        cfg->current_bb->exit_false = nullptr;
    }

    exitScope(); // end function scope
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    enterScope();
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
        if (this->ir.currentCfg->current_bb->has_return) {
            break;
        }
    }
    exitScope();
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
    string irName = declareScoped(varName);

    if (ctx->expr()) {
        this->visit(ctx->expr());
        vector<string> v = {irName, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAffectStmt(ifccParser::AffectStmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    string irName = scopedName(varName);

    this->visit(ctx->expr());

    vector<string> v = {irName, reg};
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
    string irName = scopedName(varName);

    vector<string> v = {reg, irName};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    CFG *cfg = this->ir.currentCfg;
    BasicBlock* currentBB = cfg->current_bb;

    BasicBlock *trueBB = new BasicBlock(cfg, cfg->new_BB_name() + "_true");
    BasicBlock *afterBB = new BasicBlock(cfg, cfg->new_BB_name() + "_after");
    afterBB->exit_true = currentBB->exit_true; // inherit continuation chain

    BasicBlock *elseBB = nullptr;
    if (ctx->else_stmt()) {
        elseBB = new BasicBlock(cfg, cfg->new_BB_name() + "_else");
    }

    currentBB->exit_true = trueBB;
    currentBB->exit_false = (elseBB != nullptr) ? elseBB : afterBB;

    this->visit(ctx->expr());
    currentBB->test_var_name = reg;

    cfg->add_bb(trueBB);
    cfg->current_bb->exit_true = afterBB; // default continuation

    // Visit the if block
    this->visit(ctx->block());
    if (!cfg->current_bb->has_return) {
        cfg->current_bb->exit_true = afterBB;
    }

    if (elseBB) {
        cfg->add_bb(elseBB);
        cfg->current_bb->exit_true = afterBB; // default continuation
        this->visit(ctx->else_stmt());
        if (!cfg->current_bb->has_return) {
            cfg->current_bb->exit_true = afterBB;
        }
    }

    cfg->add_bb(afterBB); // sets current_bb = afterBB

    return 0;
}

antlrcpp::Any IRGenVisitor::visitElse_stmt(ifccParser::Else_stmtContext *ctx)
{
    this->visit(ctx->block());
    return 0;
}


antlrcpp::Any IRGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    this->visit(ctx->expr());
    vector<string> v = {reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::rtrn, IntType, v);
    this->ir.currentCfg->current_bb->has_return = true;
    this->ir.currentCfg->current_bb->exit_true = nullptr;
    this->ir.currentCfg->current_bb->exit_false = nullptr;
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

antlrcpp::Any IRGenVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx) {
    string funcName = ctx->VAR()->getText();
    auto args = ctx->expr();

    // 1. Evaluate each arg and save to temp stack slots
    vector<string> varTempNames;
    for (auto *arg : args) {
        this->visit(arg);
        string varTempName = createVariableTmp();
        vector<string> v = {varTempName, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);
        varTempNames.push_back(varTempName);
    }

    // 2. Generate call instruction with the function name and the temp stack slots as arguments
    vector<string> v = {reg, funcName};
    v.insert(v.end(), varTempNames.begin(), varTempNames.end());

    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::call, IntType, v);


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
