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
        auto argExprs = f->expr();
        for (size_t i = 0; i < argExprs.size(); i++) {
            Type argType = inferExprType(argExprs[i]);

            if (functionParamTypes.count(funcName) && i < functionParamTypes[funcName].size()) {
                Type paramType = functionParamTypes[funcName][i];
                if (argType == DoubleType && paramType == IntType) {
                    cerr << "warning: implicit conversion from 'double' to 'int' in argument "
                         << (i + 1) << " of call to '" << funcName << "'\n";
                }
            }
        }
        if (functionReturnType.count(funcName)) {
            return functionReturnType[funcName];
        }
        return IntType;
    }

    return IntType;
}
// --- Signature parsing helper ---

SymbolTableVisitor::FuncSignature SymbolTableVisitor::parseSignature(
        antlr4::tree::TerminalNode *typeNode, antlr4::tree::TerminalNode *varNode,
        ifccParser::Param_listContext *paramList) {
    FuncSignature sig;
    sig.name = varNode->getText();

    string retTypeStr = typeNode->getText();
    if (retTypeStr == "double") sig.returnType = DoubleType;
    else if (retTypeStr == "void") sig.returnType = VoidType;
    else sig.returnType = IntType;

    sig.paramCount = 0;
    if (paramList) {
        auto types = paramList->TYPE();
        sig.paramCount = types.size();
        for (size_t i = 0; i < types.size(); i++) {
            if (types[i]->getText() == "void") {
                cerr << "error: parameter has incomplete type 'void'\n";
                errorFlag = true;
                sig.paramTypes.push_back(IntType);
                continue;
            }
            Type pt = (types[i]->getText() == "double") ? DoubleType : IntType;
            sig.paramTypes.push_back(pt);
        }
    }
    return sig;
}

// --- Visitors ---

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    bool hasDefinition = false;
    for (auto *func : ctx->func()) {
        if (func->block() != nullptr) {
            hasDefinition = true;
        }
        this->visit(func);
    }
    if (!hasDefinition) {
        cerr << "error: program must contain at least one function definition\n";
        errorFlag = true;
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitFunc(ifccParser::FuncContext *ctx) {
    FuncSignature sig = parseSignature(ctx->TYPE(), ctx->VAR(), ctx->param_list());
    string funcName = sig.name;
    bool isPrototype = (ctx->block() == nullptr);

    if (isPrototype) {
        if (knownFunctions.count(funcName)) {
            bool conflict = false;
            if (functionReturnType[funcName] != sig.returnType) conflict = true;
            if (functionArgCount[funcName] != sig.paramCount) conflict = true;
            if (!conflict && functionParamTypes[funcName] != sig.paramTypes) conflict = true;

            if (conflict) {
                cerr << "error: conflicting declaration for function '" << funcName << "'\n";
                errorFlag = true;
            }
            return 0;
        }

        knownFunctions.insert(funcName);
        functionReturnType[funcName] = sig.returnType;
        functionArgCount[funcName] = sig.paramCount;
        functionParamTypes[funcName] = sig.paramTypes;
        return 0;
    }

    if (definedFunctions.count(funcName)) {
        cerr << "error: function '" << funcName << "' already defined\n";
        errorFlag = true;
    } else if (knownFunctions.count(funcName)) {
        bool conflict = false;
        if (functionReturnType[funcName] != sig.returnType) conflict = true;
        if (functionArgCount[funcName] != sig.paramCount) conflict = true;
        if (!conflict && functionParamTypes[funcName] != sig.paramTypes) conflict = true;

        if (conflict) {
            cerr << "error: conflicting types for function '" << funcName << "'\n";
            errorFlag = true;
        }
    }

    currentFunctionReturnType = sig.returnType;
    functionReturnType[funcName] = sig.returnType;
    knownFunctions.insert(funcName);
    definedFunctions.insert(funcName);

    currentFunction = funcName;
    allSymbolTables[currentFunction] = {};
    varStack.clear();
    scopeMarkers.clear();
    usedVars.clear();
    nextIndex = 0;

    auto *paramList = ctx->param_list();
    if (paramList) {
        auto params = paramList->VAR();
        functionArgCount[funcName] = params.size();
        functionParamTypes[funcName] = sig.paramTypes;

        enterScope();
        for (size_t i = 0; i < params.size(); i++) {
            declareVar(params[i]->getText(), sig.paramTypes[i]);
        }
    } else {
        functionArgCount[funcName] = 0;
        functionParamTypes[funcName] = {};
        enterScope();
    }

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
