#include "IRGenVisitor.h"
using namespace std;

string reg = "!reg";

static int getCaseValue(ifccParser::Case_valueContext *ctx) {
    return stoi(ctx->getText());
}

string IRGenVisitor::createVariableTmp() {
    return "!tmp" + to_string(cptTempVariables++);
}

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Prologue:

    // Visit all statements
    auto statements = ctx->stmt();
    for (auto *stmt : statements) {
        this->visit(stmt);
        if (this->ir.currentCfg->current_bb->has_return) {
            break;
        }
    }

    // `main` implicitly returns 0 only if control can still reach the end.
    if (!this->ir.currentCfg->current_bb->has_return) {
        vector<string> loadZero = {reg, "0"};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, loadZero);

        vector<string> retZero = {reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::rtrn, IntType, retZero);
        this->ir.currentCfg->current_bb->has_return = true;
        this->ir.currentCfg->current_bb->exit_true = nullptr;
        this->ir.currentCfg->current_bb->exit_false = nullptr;
    }

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

antlrcpp::Any IRGenVisitor::visitAffectStmt(ifccParser::AffectStmtContext *ctx)
{
    string varName = ctx->VAR()->getText();

    this->visit(ctx->expr());

    vector<string> v = {varName, reg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, v);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitConst(ifccParser::ConstContext *ctx) 
{
    int val;
    if (ctx->CONST()) {
        val = stoi(ctx->CONST()->getText());
    } else {
        string token = ctx->CHAR_CONST()->getText(); // e.g. "'a'"
        if (token[1] == '\\') {
            switch(token[2]) {
                case 'n':  val = '\n'; break;
                case 't':  val = '\t'; break;
                case 'r':  val = '\r'; break;
                case '\\': val = '\\'; break;
                case '\'': val = '\''; break;
                case '0':  val = '\0'; break;
                default:   val = token[2]; break;
            }
        } else {
            val = token[1];
        }
    }
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
        if (this->ir.currentCfg->current_bb) {
            if (this->ir.currentCfg->current_bb->has_return) {
            break;
        }
        this->ir.currentCfg->current_bb->exit_true = nextBB;
        }
    }

    if (elseBB) {
        cfg->add_bb(elseBB);
        this->visit(ctx->else_stmt());
        
        if (this->ir.currentCfg->current_bb) {
            if (!this->ir.currentCfg->current_bb->has_return) {
            this->ir.currentCfg->current_bb->exit_true = nextBB;
        }
        }
    }

    cfg->current_bb = nextBB;

    return 0;
}

antlrcpp::Any IRGenVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx)
{
    CFG *cfg = this->ir.currentCfg;
    BasicBlock *entryBB = cfg->current_bb;
    BasicBlock *afterSwitch = new BasicBlock(cfg, cfg->new_BB_name() + "_switch_after");

    this->breakTriggered = false;

    this->visit(ctx->expr());

    string switchValue = createVariableTmp();
    vector<string> saveSwitchValue = {switchValue, reg};
    entryBB->add_IRInstr(IRInstr::copy, IntType, saveSwitchValue);

    auto clauses = ctx->switch_clause();
    if (clauses.empty()) {
        entryBB->exit_true = afterSwitch;
        entryBB->exit_false = nullptr;
        cfg->add_bb(afterSwitch);
        cfg->current_bb = afterSwitch;
        return 0;
    }

    vector<BasicBlock*> clauseBodies;
    vector<int> caseClauseIndexes;
    vector<BasicBlock*> caseTests;
    int defaultClauseIndex = -1;

    for (size_t i = 0; i < clauses.size(); ++i) {
        clauseBodies.push_back(new BasicBlock(cfg, cfg->new_BB_name() + "_switch_clause"));
        if (clauses[i]->case_label()) {
            caseClauseIndexes.push_back(static_cast<int>(i));
            caseTests.push_back(new BasicBlock(cfg, cfg->new_BB_name() + "_switch_test"));
        } else if (clauses[i]->default_label()) {
            defaultClauseIndex = static_cast<int>(i);
        }
    }

    BasicBlock *defaultBB = (defaultClauseIndex >= 0) ? clauseBodies[defaultClauseIndex] : afterSwitch;
    BasicBlock *firstDispatchBB = !caseTests.empty() ? caseTests.front() : defaultBB;

    entryBB->exit_true = firstDispatchBB;
    entryBB->exit_false = nullptr;

    cfg->push_break_target(afterSwitch);

    for (size_t i = 0; i < caseTests.size(); ++i) {
        BasicBlock *testBB = caseTests[i];
        BasicBlock *caseBodyBB = clauseBodies[caseClauseIndexes[i]];
        BasicBlock *nextTestBB = (i + 1 < caseTests.size()) ? caseTests[i + 1] : defaultBB;

        cfg->add_bb(testBB);

        string caseValueVar = createVariableTmp();
        vector<string> loadCaseValue = {
            caseValueVar,
            to_string(getCaseValue(clauses[caseClauseIndexes[i]]->case_label()->case_value()))
        };
        testBB->add_IRInstr(IRInstr::ldconst, IntType, loadCaseValue);

        vector<string> compareCase = {reg, switchValue, caseValueVar};
        testBB->add_IRInstr(IRInstr::cmp_eq, IntType, compareCase);
        testBB->test_var_name = reg;
        testBB->exit_true = caseBodyBB;
        testBB->exit_false = nextTestBB;
    }

    for (size_t i = 0; i < clauses.size(); ++i) {
        BasicBlock *bodyBB = clauseBodies[i];
        BasicBlock *nextBodyBB = (i + 1 < clauseBodies.size()) ? clauseBodies[i + 1] : afterSwitch;

        cfg->add_bb(bodyBB);
        bodyBB->exit_true = nextBodyBB;

        for (auto *stmt : clauses[i]->stmt()) {
            this->visit(stmt);
            if (this->ir.currentCfg->current_bb->has_return || this->breakTriggered) {
                break;
            }
            if (this->ir.currentCfg->current_bb->exit_true == nullptr) {
                this->ir.currentCfg->current_bb->exit_true = nextBodyBB;
            }
        }

        if (this->breakTriggered) {
            this->breakTriggered = false;
        } else if (!this->ir.currentCfg->current_bb->has_return) {
            this->ir.currentCfg->current_bb->exit_true = nextBodyBB;
        }
    }

    cfg->pop_break_target();
    cfg->add_bb(afterSwitch);
    cfg->current_bb = afterSwitch;

    return 0;
}

antlrcpp::Any IRGenVisitor::visitElse_stmt(ifccParser::Else_stmtContext *ctx)
{
    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
        if (this->ir.currentCfg->current_bb && this->ir.currentCfg->current_bb->has_return) {
            break;
        }
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBreak_stmt(ifccParser::Break_stmtContext *ctx)
{
    BasicBlock *breakTarget = this->ir.currentCfg->get_break_target();
    if (breakTarget != nullptr) {
        this->ir.currentCfg->current_bb->exit_true = breakTarget;
        this->ir.currentCfg->current_bb->exit_false = nullptr;
        this->breakTriggered = true;
    }
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
