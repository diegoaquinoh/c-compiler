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

void SymbolTableVisitor::declareVar(const std::string &name, Type t, bool isArray, int arraySize) {
    // Check for redeclaration within current scope only
    int scopeStart = scopeMarkers.back();
    for (int i = scopeStart; i < (int)varStack.size(); i++) {
        if (varStack[i].name == name) {
            cerr << "error: variable '" << name << "' declared multiple times in same scope\n";
            errorFlag = true;
            return;
        }
    }

    varStack.push_back({name, t, isArray, arraySize});
    // Also add to the flat symbol table for IRGenVisitor
    allSymbolTables[currentFunction][name] = t;
}

void SymbolTableVisitor::useVar(const std::string &name) {
    // Search from back to front
    bool found = false;
    for (int i = (int)varStack.size() - 1; i >= 0; i--) {
        if (varStack[i].name == name) {
            usedVars.insert(name);
            return;
        }
    }
    // Si on est là, on a parcouru tout le stack sans trouver la variable
    cerr << "error: variable '" << name << "' used before declaration\n";
    errorFlag = true;
}

Type SymbolTableVisitor::getVarType(const std::string &name) const {
    const VarInfo *var = lookupVar(name);
    return var->type;
}

bool SymbolTableVisitor::isVarArray(const std::string &name) const {
    const VarInfo *var = lookupVar(name);
    return var != nullptr && var->isArray;
}

const SymbolTableVisitor::VarInfo *SymbolTableVisitor::lookupVar(const string &name) const {
    for (int i = (int)varStack.size() - 1; i >= 0; i--) {
        if (varStack[i].name == name) {
            return &varStack[i];
        }
    }
    return nullptr;
}

bool SymbolTableVisitor::isLvalueExpr(ifccParser::ExprContext *ctx) const {
    // Soit c'est une variable soit un arrayAccess (tab[])
    return dynamic_cast<ifccParser::VarContext *>(ctx) != nullptr |
        dynamic_cast<ifccParser::ArrayAccessContext *>(ctx) != nullptr;
}

/**
 * inferExprType permet de récupérer le "type final" d'une expression
 * Mais aussi de vérifier que l'expression respecte bien les conventions qu'on s'est posé
 * qu'il n'y a pas d'effet de bord et positionner errorFlag quand il le faut.
 */
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
        if (isVarArray(name)) {
            cerr << "error: array '" << name << "' cannot be used as a scalar expression\n";
            errorFlag = true;
        }
        return getVarType(name);
    }

    if (auto *a = dynamic_cast<ifccParser::ArrayAccessContext *>(ctx)) {
        string name = a->VAR()->getText();
        useVar(name);
        // L'accès [] est autorisé uniquement pour un tableau
        if (!isVarArray(name)) {
            cerr << "error: variable '" << name << "' is not an array\n";
            errorFlag = true;
        }

        // L'indice doit être un entier 
        Type idxType = inferExprType(a->expr());
        if (idxType != IntType) {
            cerr << "error: array index for '" << name << "' must be of type int\n";
            errorFlag = true;
        }
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
        return (lhs == DoubleType || rhs == DoubleType) ? DoubleType : IntType;
    }

    if (auto *r = dynamic_cast<ifccParser::RelationalContext *>(ctx)) {
        // Les comparaisons produisent toujours un entier (bool 0 ou 1)
        inferExprType(r->expr(0));
        inferExprType(r->expr(1));
        return IntType;
    }

    if (auto *e = dynamic_cast<ifccParser::EqualityContext *>(ctx)) {
        // Les comparaisons produisent toujours un booleen entier (0 ou 1)
        inferExprType(e->expr(0));
        inferExprType(e->expr(1));
        return IntType;
    }

    if (auto *b = dynamic_cast<ifccParser::BitwiseandContext *>(ctx)) {
        // Les operateurs bitwise sont reserves aux operandes entiers
        Type lhs = inferExprType(b->expr(0));
        Type rhs = inferExprType(b->expr(1));
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
        if (lhs != IntType || rhs != IntType) {
            cerr << "error: bitwise '|' only supports int operands\n";
            errorFlag = true;
        }
        return IntType;
    }

    if (auto *a = dynamic_cast<ifccParser::AffectStmtContext *>(ctx)) {
        // Avec la regle expr '=' expr, la validation de la lvalue se fait ici
        if (!isLvalueExpr(a->expr(0))) {
            cerr << "error: left-hand side of assignment is not an lvalue\n";
            errorFlag = true;
        }

        Type lhsType = inferExprType(a->expr(0));
        Type rhsType = inferExprType(a->expr(1));

        if (dynamic_cast<ifccParser::VarContext *>(a->expr(0)) != nullptr) {
            auto *lhsVar = static_cast<ifccParser::VarContext *>(a->expr(0));
            string lhsName = lhsVar->VAR()->getText();
            if (isVarArray(lhsName)) {
                cerr << "error: array '" << lhsName << "' cannot be assigned as a scalar\n";
                errorFlag = true;
            }
        }

        if (rhsType == DoubleType && lhsType == IntType) {
            // Allowed by existing semantics (implicit narrowing in current project rules).
        }

        return lhsType;
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
    // On memorise le type de declaration pour tous les decl_item de l'instruction
    currentDeclType = (ctx->TYPE()->getText() == "double") ? DoubleType : IntType;
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx) {
    string varName = ctx->VAR()->getText();
    bool isArray = ctx->CONST() != nullptr;
    int arraySize = 0;
    if (isArray) {
        arraySize = stoi(ctx->CONST()->getText());
        // On force une taille strictement positive pour eviter des offsets invalides en generation
        // Gcc autorise une taille nulle mais pas nous : ça hérite de vieilles retro-compatibilités
        if (arraySize <= 0) {
            cerr << "error: array '" << varName << "' size must be positive\n";
            errorFlag = true;
        }

        if (ctx->expr()) {
            // On a une initialialisation alors que c'est un tableau => problème
            cerr << "error: array '" << varName << "' initializer is not supported\n";
            errorFlag = true;
        }
    }

    declareVar(varName, currentDeclType, isArray, arraySize);

    if (ctx->expr()) {
        inferExprType(ctx->expr());
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
    inferExprType(ctx);
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
    if (ctx->expr()) {
        inferExprType(ctx->expr());
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
