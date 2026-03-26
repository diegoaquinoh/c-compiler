#include "SymbolTableVisitor.h"
using namespace std;

// --- Scope management ---

void SymbolTableVisitor::enterScope() {
    scopeMarkers.push_back(varStack.size());
}

void SymbolTableVisitor::exitScope() {
    int marker = scopeMarkers.back();
    scopeMarkers.pop_back();
    varStack.resize(marker);
}

int SymbolTableVisitor::declareVar(const std::string &name) {
    // Check for redeclaration within current scope only
    int scopeStart = scopeMarkers.back();
    for (int i = scopeStart; i < (int)varStack.size(); i++) {
        if (varStack[i].first == name) {
            cerr << "error: variable '" << name << "' declared multiple times in same scope\n";
            errorFlag = true;
            return varStack[i].second;
        }
    }
    nextIndex -= 4;
    varStack.push_back({name, nextIndex});
    // Also add to the flat symbol table for IRGenVisitor
    allSymbolTables[currentFunction][name] = nextIndex;
    return nextIndex;
}

int SymbolTableVisitor::lookupVar(const std::string &name) {
    // Search from back to front (innermost scope first)
    for (int i = (int)varStack.size() - 1; i >= 0; i--) {
        if (varStack[i].first == name) {
            return varStack[i].second;
        }
    }
    return 0; // not found
}

void SymbolTableVisitor::useVar(const std::string &name) {
    // Search from back to front
    bool found = false;
    for (int i = (int)varStack.size() - 1; i >= 0; i--) {
        if (varStack[i].first == name) {
            found = true;
            break;
        }
    }
    if (!found) {
        cerr << "error: variable '" << name << "' used before declaration\n";
        errorFlag = true;
    } else {
        usedVars.insert(name);
    }
}

// --- Visitors ---

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    for (auto *func : ctx->func_def()) {
        this->visit(func);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFunc_def(ifccParser::Func_defContext *ctx) {
    string funcName = ctx->VAR()->getText();
    if (knownFunctions.count(funcName)) {
        cerr << "error: function '" << funcName << "' already declared\n";
        errorFlag = true;
    }

    currentFunction = funcName;
    allSymbolTables[currentFunction] = {};
    varStack.clear();
    scopeMarkers.clear();
    usedVars.clear();
    nextIndex = 0;

    // Register this function so other functions can call it
    knownFunctions.insert(funcName);

    // Count parameters
    int paramCount = 0;
    if (ctx->param_list()) {
        auto params = ctx->param_list()->VAR();
        paramCount = params.size();
        functionArgCount[funcName] = paramCount;

        // Enter function scope and declare parameters
        enterScope();
        for (auto *param : params) {
            declareVar(param->getText());
        }
    } else {
        functionArgCount[funcName] = 0;
        enterScope();
    }

    // Visit the function body block (block will handle its own scope)
    this->visit(ctx->block());

    for (auto &[name, idx] : allSymbolTables[currentFunction]) {
        (void)idx;
        if (!usedVars.count(name)) {
            cerr << "warning: variable '" << name << "' declared but never used\n";
        }
    }

    exitScope();
    currentFunction = "";
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx) {
    enterScope();
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    exitScope();
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) {
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx) {
    declareVar(ctx->VAR()->getText());
    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) {
    this->visit(ctx->expr());
    enterScope();
    for (auto *clause : ctx->switch_clause()) {
        this->visit(clause);
    }
    exitScope();
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitSwitch_clause(ifccParser::Switch_clauseContext *ctx) {
    if (ctx->case_label()) {
        this->visit(ctx->case_label());
    }
    if (ctx->default_label()) {
        this->visit(ctx->default_label());
    }
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitCase_label(ifccParser::Case_labelContext *ctx) {
    this->visit(ctx->case_value());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDefault_label(ifccParser::Default_labelContext *ctx) {
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitCase_value(ifccParser::Case_valueContext *ctx) {
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBreak_stmt(ifccParser::Break_stmtContext *ctx) {
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAffectStmt(ifccParser::AffectStmtContext *ctx) {
    useVar(ctx->VAR()->getText());
    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx) {
    this->visit(ctx->expr());
    this->visit(ctx->block());
    if (ctx->else_stmt()) {
        this->visit(ctx->else_stmt());
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitElse_stmt(ifccParser::Else_stmtContext *ctx) {
    this->visit(ctx->block());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx) {
    string funcName = ctx->VAR()->getText();
    if (!knownFunctions.count(funcName)) {
        cerr << "error: function '" << funcName << "' called but not declared\n";
        errorFlag = true;
    }
    int argCount = ctx->expr().size();
    if (argCount > 6) {
        cerr << "error: function '" << funcName << "' called with too many arguments (" << argCount << ")\n";
        errorFlag = true;
    } else if (functionArgCount.count(funcName) && argCount != functionArgCount[funcName]) {
        cerr << "error: '" << funcName << "' expects " << functionArgCount[funcName]
             << " arguments, got " << argCount << "\n";
        errorFlag = true;
    }
    for (auto *arg : ctx->expr()) {
        this->visit(arg);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitMultdiv(ifccParser::MultdivContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAddsub(ifccParser::AddsubContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitNegative(ifccParser::NegativeContext *ctx) {
    ifccParser::ExprContext *operand = ctx->expr();

    if (dynamic_cast<ifccParser::NegativeContext *>(operand) != nullptr) {
        cerr << "error: double negation is not allowed\n";
        errorFlag = true;
        return 0;
    }

    this->visit(operand);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitLogicalnot(ifccParser::LogicalnotContext *ctx){
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitParens(ifccParser::ParensContext *ctx) {
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitConst(ifccParser::ConstContext *ctx) {
    (void)ctx;

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVar(ifccParser::VarContext *ctx) {
    useVar(ctx->VAR()->getText());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitRelational(ifccParser::RelationalContext *ctx){
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitEquality(ifccParser::EqualityContext *ctx){
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) {
    this->visit(ctx->expr());
    enterScope();
    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    exitScope();
    return 0;
}
