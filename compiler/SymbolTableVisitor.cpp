#include "SymbolTableVisitor.h"
using namespace std;

int SymbolTableVisitor::declareVar(const std::string &name) {
    if (symbolTable.count(name)) {
        cerr << "error: variable '" << name << "' declared multiple times\n";
        errorFlag = true;
        return symbolTable[name]; 
    }
    nextIndex -= 4;
    symbolTable[name] = nextIndex;
    return nextIndex;
}

void SymbolTableVisitor::useVar(const std::string &name) {
    if (!symbolTable.count(name)) {
        cerr << "error: variable '" << name << "' used before declaration\n";
        errorFlag = true;
    } else {
        usedVars.insert(name);
    }
}

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    // Visit all statements and the return
    for (auto *stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    this->visit(ctx->return_stmt());

    // Check that every declared variable is used at least once
    for (auto &[name, idx] : symbolTable) {
        if (!usedVars.count(name)) {
            cerr << "warning: variable '" << name << "' declared but never used\n";
        }
    }
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
        //useVar(ctx->expr()->VAR()->getText());
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitExpr_stmt(ifccParser::Expr_stmtContext *ctx) {
    visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx) {
    // Validate function name against knownFunctions
    // Check arg count (putchar=1, getchar=0, max 6 for any function)
    // Visit each argument expression recursively (for variable usage tracking)
    string funcName = ctx->IDENT()->getText();
    if (!knownFunctions.count(funcName)) {
        cerr << "error: function '" << funcName << "' called but not declared\n";
        errorFlag = true;
    }
    int argCount = ctx->expr().size();
    if (argCount > 6) {
        cerr << "error: function '" << funcName << "' called with too many arguments (" << argCount << ")\n";
        errorFlag = true;
    } else if (knownFunctions.count(funcName) && argCount != knownFunctions[funcName]) {
        cerr << "error: '" << funcName << "' expects " << knownFunctions[funcName]
             << " arguments, got " << argCount << "\n";
        errorFlag = true;
    }

    for (auto *arg : ctx->expr()) {
        this->visit(arg);
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) {
    useVar(ctx->VAR()->getText());
    if (ctx->expr()) {
        //useVar(ctx->expr()->VAR()->getText());
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    if (ctx->expr()) {
        //useVar(ctx->expr()->VAR()->getText());
        this->visit(ctx->expr());
    }
    return 0;
}