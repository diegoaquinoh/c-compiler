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

antlrcpp::Any SymbolTableVisitor::visitDeclList(ifccParser::DeclListContext *ctx) {
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDeclItemVoid(ifccParser::DeclItemVoidContext *ctx) {
    declareVar(ctx->VAR()->getText());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDeclItemConst(ifccParser::DeclItemConstContext *ctx) {
    declareVar(ctx->VAR()->getText());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDeclItemVar(ifccParser::DeclItemVarContext *ctx) {
    useVar(ctx->VAR(1)->getText());
    declareVar(ctx->VAR(0)->getText());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAffectConst(ifccParser::AffectConstContext *ctx) {
    useVar(ctx->VAR()->getText());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAffectVar(ifccParser::AffectVarContext *ctx) {
    useVar(ctx->VAR(0)->getText());
    useVar(ctx->VAR(1)->getText()); 
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturnConst(ifccParser::ReturnConstContext *ctx) {
    // Nothing to do
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx) {
    useVar(ctx->VAR()->getText());
    return 0;
}