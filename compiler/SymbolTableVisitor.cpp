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
    symbolType[name] = IntType;
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
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) {
    useVar(ctx->VAR()->getText());
    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    return 0;
}

// antlrcpp::Any SymbolTableVisitor::visitCallStmt(ifccParser::CallStmtContext *ctx) {
//     // Reuse funcCall validation logic: check function name, arg count, visit args
//     string funcName = ctx->VAR()->getText();
//     if (!knownFunctions.count(funcName)) {
//         cerr << "error: function '" << funcName << "' called but not declared\n";
//         errorFlag = true;
//     }
//     int argCount = ctx->expr().size();
//     if (argCount > 6) {
//         cerr << "error: function '" << funcName << "' called with too many arguments (" << argCount << ")\n";
//         errorFlag = true;
//     } else if (knownFunctions.count(funcName) && argCount != functionArgCount[funcName]) {
//         cerr << "error: '" << funcName << "' expects " << functionArgCount[funcName]
//              << " arguments, got " << argCount << "\n";
//         errorFlag = true;
//     }
//     for (auto *arg : ctx->expr()) {
//         this->visit(arg);
//     }
//     return 0;
// }

// antlrcpp::Any SymbolTableVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx) {
//     string funcName = ctx->VAR()->getText();
//     if (!knownFunctions.count(funcName)) {
//         cerr << "error: function '" << funcName << "' called but not declared\n";
//         errorFlag = true;
//     }
//     int argCount = ctx->expr().size();
//     if (argCount > 6) {
//         cerr << "error: function '" << funcName << "' called with too many arguments (" << argCount << ")\n";
//         errorFlag = true;
//     } else if (knownFunctions.count(funcName) && argCount != functionArgCount[funcName]) {
//         cerr << "error: '" << funcName << "' expects " << functionArgCount[funcName]
//              << " arguments, got " << argCount << "\n";
//         errorFlag = true;
//     }
//     for (auto *arg : ctx->expr()) {
//         this->visit(arg);
//     }
//     return 0;
// }

antlrcpp::Any SymbolTableVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitNegative(ifccParser::NegativeContext *ctx){
    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitParens(ifccParser::ParensContext *ctx){
    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVar(ifccParser::VarContext *ctx) {
    useVar(ctx->VAR()->getText());

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));

    return 0;
}