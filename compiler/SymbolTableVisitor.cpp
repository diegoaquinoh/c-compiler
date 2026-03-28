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

void SymbolTableVisitor::declareVar(const std::string &name, Type t) {
    // Check for redeclaration within current scope only
    int scopeStart = scopeMarkers.back();
    for (int i = scopeStart; i < (int)varStack.size(); i++) {
        if (varStack[i].first == name) {
            cerr << "error: variable '" << name << "' declared multiple times in same scope\n";
            errorFlag = true;
            return;
        }
    }

    varStack.push_back({name, t});
    // Also add to the flat symbol table for IRGenVisitor
    allSymbolTables[currentFunction][name] = t;
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

Type SymbolTableVisitor::getVarType(const std::string &name) const {
    for (int i = (int)varStack.size() - 1; i >= 0; i--) {
        if (varStack[i].first == name) {
            return varStack[i].second;
        }
    }
    return IntType;
}

Type SymbolTableVisitor::inferExprType(ifccParser::ExprContext *ctx) {
    if (ctx == nullptr) {
        return IntType;
    }

    if (auto *c = dynamic_cast<ifccParser::ConstContext *>(ctx)) {
        if (c->DOUBLE_CONST()) {
            return DoubleType;
        }
        return IntType;
    }

    if (auto *v = dynamic_cast<ifccParser::VarContext *>(ctx)) {
        string name = v->VAR()->getText();
        useVar(name);
        return getVarType(name);
    }

    if (auto *p = dynamic_cast<ifccParser::ParensContext *>(ctx)) {
        return inferExprType(p->expr());
    }

    if (auto *n = dynamic_cast<ifccParser::NegativeContext *>(ctx)) {
        return inferExprType(n->expr());
    }

    if (auto *n = dynamic_cast<ifccParser::LogicalnotContext *>(ctx)) {
        inferExprType(n->expr());
        return IntType;
    }

    if (auto *m = dynamic_cast<ifccParser::MultdivContext *>(ctx)) {
        // Regle arithmetique : int op double => double, sauf modulo
        Type lhs = inferExprType(m->expr(0));
        Type rhs = inferExprType(m->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
            return IntType;
        }
        string op = m->OP->getText();
        if (op == "%" && (lhs == DoubleType || rhs == DoubleType)) {
            cerr << "error: operator '%' only supports int operands\n";
            errorFlag = true;
        }
        return (lhs == DoubleType || rhs == DoubleType) ? DoubleType : IntType;
    }

    if (auto *a = dynamic_cast<ifccParser::AddsubContext *>(ctx)) {
        // Meme regle de pour + et -
        Type lhs = inferExprType(a->expr(0));
        Type rhs = inferExprType(a->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
            return IntType;
        }
        return (lhs == DoubleType || rhs == DoubleType) ? DoubleType : IntType;
    }

    if (auto *r = dynamic_cast<ifccParser::RelationalContext *>(ctx)) {
        // Les comparaisons produisent toujours un entier (bool 0 ou 1)
        Type lhs = inferExprType(r->expr(0));
        Type rhs = inferExprType(r->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
        return IntType;
    }

    if (auto *e = dynamic_cast<ifccParser::EqualityContext *>(ctx)) {
        // Les comparaisons produisent toujours un booleen entier (0 ou 1)
        Type lhs = inferExprType(e->expr(0));
        Type rhs = inferExprType(e->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
        return IntType;
    }

    if (auto *b = dynamic_cast<ifccParser::BitwiseandContext *>(ctx)) {
        // Les operateurs bitwise sont reserves aux operandes entiers
        Type lhs = inferExprType(b->expr(0));
        Type rhs = inferExprType(b->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
            return IntType;
        }
        if (lhs != IntType || rhs != IntType) {
            cerr << "error: bitwise '&' only supports int operands\n";
            errorFlag = true;
        }
        return IntType;
    }

    if (auto *b = dynamic_cast<ifccParser::BitwisexorContext *>(ctx)) {
        // Les operateurs bitwise sont reserves aux operandes entiers
        Type lhs = inferExprType(b->expr(0));
        Type rhs = inferExprType(b->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
            return IntType;
        }
        if (lhs != IntType || rhs != IntType) {
            cerr << "error: bitwise '^' only supports int operands\n";
            errorFlag = true;
        }
        return IntType;
    }

    if (auto *b = dynamic_cast<ifccParser::BitwiseorContext *>(ctx)) {
        // Les operateurs bitwise sont reserves aux operandes entiers
        Type lhs = inferExprType(b->expr(0));
        Type rhs = inferExprType(b->expr(1));
        if (lhs == VoidType || rhs == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
            return IntType;
        }
        if (lhs != IntType || rhs != IntType) {
            cerr << "error: bitwise '|' only supports int operands\n";
            errorFlag = true;
        }
        return IntType;
    }

    if (auto *a = dynamic_cast<ifccParser::AffectStmtContext *>(ctx)) {
        string name = a->VAR()->getText();
        useVar(name);
        if (a->expr()) {
            inferExprType(a->expr());
        }
        return getVarType(name);
    }

    if (auto *f = dynamic_cast<ifccParser::FuncCallContext *>(ctx)) {
        string funcName = f->VAR()->getText();
        if (!knownFunctions.count(funcName)) {
            cerr << "error: function '" << funcName << "' called but not declared\n";
            errorFlag = true;
        }
        int argCount = static_cast<int>(f->expr().size());
        if (argCount > 6) {
            cerr << "error: function '" << funcName << "' called with too many arguments (" << argCount << ")\n";
            errorFlag = true;
        } else if (knownFunctions.count(funcName) && argCount != functionArgCount[funcName]) {
            cerr << "error: '" << funcName << "' expects " << functionArgCount[funcName]
                 << " arguments, got " << argCount << "\n";
            errorFlag = true;
        }
        for (auto *arg : f->expr()) {
            inferExprType(arg);
        }
        if (functionReturnType.count(funcName)) {
            return functionReturnType[funcName];
        }
        return IntType;
    }

    return IntType;
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

    // Parse and store return type
    string retTypeStr = ctx->TYPE()->getText();
    if (retTypeStr == "double") currentFunctionReturnType = DoubleType;
    else if (retTypeStr == "void") currentFunctionReturnType = VoidType;
    else currentFunctionReturnType = IntType;
    functionReturnType[funcName] = currentFunctionReturnType;

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
        auto types = ctx->param_list()->TYPE();
        paramCount = params.size();
        functionArgCount[funcName] = paramCount;

        // Enter function scope and declare parameters
        enterScope();
        for (size_t i = 0; i < params.size(); i++) {
            if (types[i]->getText() == "void") {
                cerr << "error: parameter '" << params[i]->getText() << "' has incomplete type 'void'\n";
                errorFlag = true;
                continue;
            }
            Type paramType = (types[i]->getText() == "double") ? DoubleType : IntType;
            declareVar(params[i]->getText(), paramType);
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
    if (ctx->TYPE()->getText() == "void") {
        cerr << "error: variable has incomplete type 'void'\n";
        errorFlag = true;
        return 0;
    }
    // On memorise le type de declaration pour tous les decl_item de l'instruction
    currentDeclType = (ctx->TYPE()->getText() == "double") ? DoubleType : IntType;
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx) {
    string varName = ctx->VAR()->getText();
    declareVar(varName, currentDeclType);
    if (ctx->expr()) {
        Type exprType = inferExprType(ctx->expr());
        if (exprType == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) {
    // Semantique volontairement simple et proche de C : l'expression du switch doit etre int.
    Type t = inferExprType(ctx->expr());
    if (t != IntType) {
        cerr << "error: switch expression must be of type int\n";
        errorFlag = true;
    }
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
    string varName = ctx->VAR()->getText();
    useVar(varName);
    if (ctx->expr()) {
        Type exprType = inferExprType(ctx->expr());
        if (exprType == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx) {
    inferExprType(ctx->expr());
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
    if (currentFunctionReturnType == VoidType) {
        if (ctx->expr()) {
            cerr << "error: void function should not return a value\n";
            errorFlag = true;
        }
    } else {
        if (ctx->expr()) {
            inferExprType(ctx->expr());
        } else {
            cerr << "error: non-void function should return a value\n";
            errorFlag = true;
        }
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx) {
    inferExprType(ctx);
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitNegative(ifccParser::NegativeContext *ctx) {
    ifccParser::ExprContext *operand = ctx->expr();

    if (dynamic_cast<ifccParser::NegativeContext *>(operand) != nullptr) {
        cerr << "error: double negation is not allowed\n";
        errorFlag = true;
        return 0;
    }

    inferExprType(operand);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitLogicalnot(ifccParser::LogicalnotContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitParens(ifccParser::ParensContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitConst(ifccParser::ConstContext *ctx) {
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVar(ifccParser::VarContext *ctx) {
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitRelational(ifccParser::RelationalContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitEquality(ifccParser::EqualityContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) {
    inferExprType(ctx->expr());
    this->visit(ctx->expr());
    enterScope();
    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    exitScope();
    return 0;
}
