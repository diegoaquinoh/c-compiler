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

void SymbolTableVisitor::declareVar(const std::string &name, Type t, bool isArray, int arraySize, Type pointeeType, int pointerDepth) {
    // Check for redeclaration within current scope only
    int scopeStart = scopeMarkers.back();
    for (int i = scopeStart; i < (int)varStack.size(); i++) {
        if (varStack[i].name == name) {
            cerr << "error: variable '" << name << "' declared multiple times in same scope\n";
            errorFlag = true;
            return;
        }
    }

    varStack.push_back({name, t, isArray, arraySize, pointeeType, pointerDepth});
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

Type SymbolTableVisitor::getVarPointeeType(const std::string &name) const {
    const VarInfo *var = lookupVar(name);
    return var->pointeeType;
}

int SymbolTableVisitor::getVarPointerDepth(const std::string &name) const {
    const VarInfo *var = lookupVar(name);
    return var->pointerDepth;
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
    // Soit c'est une variable soit un arrayAccess (tab[]), soit *ptr = ...
    return dynamic_cast<ifccParser::VarContext *>(ctx) != nullptr ||
        dynamic_cast<ifccParser::ArrayAccessContext *>(ctx) != nullptr ||
        dynamic_cast<ifccParser::DerefContext *>(ctx) != nullptr;
}

Type SymbolTableVisitor::parseBaseType(const string &typeText) const {
    if (typeText == "double") {
        return DoubleType;
    } 
    if (typeText == "void") {
        return VoidType;
    } 
    return IntType;
}

Type SymbolTableVisitor::parseDeclaredType(const string &typeText, ifccParser::Ptr_suffixContext *ptrSuffix, Type &outPointeeType, int &outPointerDepth) const {
    outPointeeType = IntType;
    outPointerDepth = 0;
    Type baseType = parseBaseType(typeText);
    string suffix = ptrSuffix ? ptrSuffix->getText() : "";
    if (suffix.empty()) {
        return baseType;
    }

    outPointerDepth = suffix.size();
    outPointeeType = baseType;
    return PointerType;
}

int SymbolTableVisitor::getPointedElementSize(const ExprTypeInfo &ptrType) const {
    if (ptrType.pointerDepth > 1) return 8;
    if (ptrType.pointeeType == DoubleType) return 8;
    if (ptrType.pointeeType == VoidType) return 1;
    return 4;
}

bool SymbolTableVisitor::isZeroLiteralExpr(ifccParser::ExprContext *ctx) const {
    auto *c = dynamic_cast<ifccParser::ConstContext *>(ctx);
    return c != nullptr && c->CONST() != nullptr && c->CONST()->getText() == "0";
}

/**
 * inferExprType permet de récupérer le "type final" d'une expression
 * Mais aussi de vérifier que l'expression respecte bien les conventions qu'on s'est posé
 * qu'il n'y a pas d'effet de bord et positionner errorFlag quand il le faut.
 */
SymbolTableVisitor::ExprTypeInfo SymbolTableVisitor::inferExprType(ifccParser::ExprContext *ctx) {
    if (ctx == nullptr) {
        return {IntType, false, IntType, 0};
    }

    if (auto *c = dynamic_cast<ifccParser::ConstContext *>(ctx)) {
        if (c->DOUBLE_CONST()) {
            return {DoubleType, false, DoubleType, 0};
        }
        bool isZero = c->CONST() && c->CONST()->getText() == "0";
        return {IntType, isZero, IntType, 0};
    }

    if (auto *v = dynamic_cast<ifccParser::VarContext *>(ctx)) {
        string name = v->VAR()->getText();
        useVar(name);
        if (isVarArray(name)) {
            cerr << "error: array '" << name << "' cannot be used as a scalar expression\n";
            errorFlag = true;
        }
        Type t = getVarType(name);
        if (t == PointerType) {
            return {PointerType, false, getVarPointeeType(name), getVarPointerDepth(name)};
        }
        return {t, false, t, 0};
    }

    if (auto *a = dynamic_cast<ifccParser::ArrayAccessContext *>(ctx)) {
        string name = a->VAR()->getText();
        useVar(name);
        if (!isVarArray(name)) {
            cerr << "error: variable '" << name << "' is not an array\n";
            errorFlag = true;
        }

        ExprTypeInfo idxType = inferExprType(a->expr());
        if (idxType.type != IntType || idxType.pointeeType != IntType) {
            cerr << "error: array index for '" << name << "' must be of type int\n";
            errorFlag = true;
        }
        Type elemType = getVarType(name);
        return {elemType, false, elemType, 0};
    }

    if (auto *p = dynamic_cast<ifccParser::ParensContext *>(ctx)) {
        return inferExprType(p->expr());
    }

    if (auto *n = dynamic_cast<ifccParser::NegativeContext *>(ctx)) {
        if (dynamic_cast<ifccParser::NegativeContext *>(n->expr()) != nullptr) {
            cerr << "error: double negation is not allowed\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }

        ExprTypeInfo t = inferExprType(n->expr());
        if (t.type == PointerType) {
            cerr << "error: invalid use of unary '-' on pointer\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        return t;
    }

    if (auto *n = dynamic_cast<ifccParser::LogicalnotContext *>(ctx)) {
        inferExprType(n->expr());
        return {IntType, false, IntType, 0};
    }

    if (auto *d = dynamic_cast<ifccParser::DerefContext *>(ctx)) {
        ExprTypeInfo ptrInfo = inferExprType(d->expr());
        if (ptrInfo.type != PointerType) {
            cerr << "error: cannot dereference non-pointer expression\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        if (ptrInfo.pointeeType == VoidType && ptrInfo.pointerDepth == 1) {
            cerr << "error: cannot dereference void pointer\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        if (ptrInfo.pointerDepth > 1) {
            return {PointerType, false, ptrInfo.pointeeType, ptrInfo.pointerDepth - 1};
        }
        return {ptrInfo.pointeeType, false, ptrInfo.pointeeType, 0};
    }

    if (auto *a = dynamic_cast<ifccParser::AddressOfContext *>(ctx)) {
        if (!isLvalueExpr(a->expr())) {
            cerr << "error: lvalue required as unary '&' operand\n";
            errorFlag = true;
            return {PointerType, false, IntType, 1};
        }
        ExprTypeInfo base = inferExprType(a->expr());
        if (base.type == PointerType) {
            return {PointerType, false, base.pointeeType, base.pointerDepth + 1};
        }
        return {PointerType, false, base.type, 1};
    }

    if (auto *m = dynamic_cast<ifccParser::MultdivContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(m->expr(0));
        ExprTypeInfo rhs = inferExprType(m->expr(1));
        if (lhs.type == VoidType || rhs.type == VoidType || lhs.type == PointerType || rhs.type == PointerType) {
            cerr << "error: invalid operands to binary operator '" << m->OP->getText() << "'\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        string op = m->OP->getText();
        if (op == "%" && (lhs.type == DoubleType || rhs.type == DoubleType)) {
            cerr << "error: operator '%' only supports int operands\n";
            errorFlag = true;
        }
        Type rt = (lhs.type == DoubleType || rhs.type == DoubleType) ? DoubleType : IntType;
        return {rt, false, rt, 0};
    }

    if (auto *a = dynamic_cast<ifccParser::AddsubContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(a->expr(0));
        ExprTypeInfo rhs = inferExprType(a->expr(1));
        if (lhs.type == VoidType || rhs.type == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        string op = a->OP->getText();
        if (lhs.type == PointerType && rhs.type == PointerType) {
            // On a pas le droit de faire ptr + ptr, seulement ptr - ptr
            if (op == "-") {
                bool samePtr = lhs.pointerDepth == rhs.pointerDepth && lhs.pointeeType == rhs.pointeeType;
                bool voidCompat = lhs.pointerDepth == 1 && rhs.pointerDepth == 1 &&
                                  (lhs.pointeeType == VoidType || rhs.pointeeType == VoidType);
                if (!(samePtr || voidCompat)) {
                    cerr << "error: invalid subtraction between incompatible pointer types\n";
                    errorFlag = true;
                }
                return {IntType, false, IntType, 0};
            }
            cerr << "error: invalid operands to binary operator '+'\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        if (lhs.type == PointerType && rhs.type == IntType) {
            // On peut faire + ou - peu importe
            if (lhs.pointeeType == VoidType && lhs.pointerDepth == 1) {
                cerr << "error: pointer arithmetic on void* is not supported\n";
                errorFlag = true;
            }

            return lhs;
        }
        if (lhs.type == IntType && rhs.type == PointerType) {
            // + uniquement
            if (op == "+") {
                return rhs;
            }
            cerr << "error: invalid operands to binary operator '-'\n";
            errorFlag = true;
            return {IntType, false, IntType, 0};
        }
        // Les cas int et double : promotion
        Type rt = (lhs.type == DoubleType || rhs.type == DoubleType) ? DoubleType : IntType;
        return {rt, false, rt, 0};
    }

    if (auto *r = dynamic_cast<ifccParser::RelationalContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(r->expr(0));
        ExprTypeInfo rhs = inferExprType(r->expr(1));
        if (lhs.type == VoidType || rhs.type == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
        if (lhs.type == PointerType && rhs.type == PointerType) {
            // Uniquement autorisé pour les mêmes types de pointeur, ie même profondeur et même type
            if (lhs.pointerDepth != rhs.pointerDepth || lhs.pointeeType != rhs.pointeeType) {
                cerr << "error: invalid comparison between incompatible pointer types\n";
                errorFlag = true;
            }
        } else if (lhs.type == PointerType || rhs.type == PointerType) {
            cerr << "error: invalid comparison between pointer and non-pointer\n";
            errorFlag = true;
        }
        return {IntType, false, IntType, 0};
    }

    if (auto *e = dynamic_cast<ifccParser::EqualityContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(e->expr(0));
        ExprTypeInfo rhs = inferExprType(e->expr(1));
        if (lhs.type == VoidType || rhs.type == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
        if (lhs.type == PointerType || rhs.type == PointerType) {
            // Ici on autorise avec : les mêmes types de pointeur, 
            // les pointeurs génériques (void*) et pointeurs nuls (0)
            bool nullMix = (lhs.type == PointerType && rhs.type == IntType && rhs.isZeroLiteral) ||
                           (rhs.type == PointerType && lhs.type == IntType && lhs.isZeroLiteral);
            bool samePtr = lhs.type == PointerType && rhs.type == PointerType &&
                           lhs.pointerDepth == rhs.pointerDepth && lhs.pointeeType == rhs.pointeeType;
            bool voidCompat = lhs.type == PointerType && rhs.type == PointerType &&
                              lhs.pointerDepth == 1 && rhs.pointerDepth == 1 &&
                              (lhs.pointeeType == VoidType || rhs.pointeeType == VoidType);
            if (!(nullMix || samePtr || voidCompat)) {
                cerr << "error: invalid comparison between pointer and non-pointer\n";
                errorFlag = true;
            }
        }
        return {IntType, false, IntType, 0};
    }

    if (auto *b = dynamic_cast<ifccParser::BitwiseandContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(b->expr(0));
        ExprTypeInfo rhs = inferExprType(b->expr(1));
        if (lhs.type != IntType || rhs.type != IntType) {
            cerr << "error: bitwise '&' only supports int operands\n";
            errorFlag = true;
        }
        return {IntType, false, IntType, 0};
    }

    if (auto *b = dynamic_cast<ifccParser::BitwisexorContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(b->expr(0));
        ExprTypeInfo rhs = inferExprType(b->expr(1));
        if (lhs.type != IntType || rhs.type != IntType) {
            cerr << "error: bitwise '^' only supports int operands\n";
            errorFlag = true;
        }
        return {IntType, false, IntType, 0};
    }

    if (auto *b = dynamic_cast<ifccParser::BitwiseorContext *>(ctx)) {
        ExprTypeInfo lhs = inferExprType(b->expr(0));
        ExprTypeInfo rhs = inferExprType(b->expr(1));
        if (lhs.type != IntType || rhs.type != IntType) {
            cerr << "error: bitwise '|' only supports int operands\n";
            errorFlag = true;
        }
        return {IntType, false, IntType, 0};
    }

    if (auto *a = dynamic_cast<ifccParser::AffectStmtContext *>(ctx)) {
        if (!isLvalueExpr(a->expr(0))) {
            cerr << "error: left-hand side of assignment is not an lvalue\n";
            errorFlag = true;
        }

        ExprTypeInfo lhsType = inferExprType(a->expr(0));
        ExprTypeInfo rhsType = inferExprType(a->expr(1));

        if (dynamic_cast<ifccParser::VarContext *>(a->expr(0)) != nullptr) {
            auto *lhsVar = static_cast<ifccParser::VarContext *>(a->expr(0));
            string lhsName = lhsVar->VAR()->getText();
            if (isVarArray(lhsName)) {
                cerr << "error: array '" << lhsName << "' cannot be assigned as a scalar\n";
                errorFlag = true;
            }
        }

        if (lhsType.type == PointerType) {
            // le ptr de droite est du même type que la lvalue
            bool samePtr = rhsType.type == PointerType && rhsType.pointeeType == lhsType.pointeeType && rhsType.pointerDepth == lhsType.pointerDepth;
            // pointeur générique (void*)
            bool voidCompat = rhsType.type == PointerType && rhsType.pointerDepth == 1 && lhsType.pointerDepth == 1 &&
                              (rhsType.pointeeType == VoidType || lhsType.pointeeType == VoidType);
            // pointeur null (0)
            bool ptrNull = (rhsType.type == IntType && rhsType.isZeroLiteral);
            if (!(samePtr || voidCompat || ptrNull)) {
                cerr << "error: incompatible assignment to pointer\n";
                errorFlag = true;
            }
            return lhsType;
        }

        if (rhsType.type == PointerType) {
            cerr << "error: incompatible assignment from pointer to non-pointer\n";
            errorFlag = true;
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
            ExprTypeInfo argType = inferExprType(argExprs[i]);

            if (functionParamTypes.count(funcName) && i < functionParamTypes[funcName].size()) {
                Type paramType = functionParamTypes[funcName][i];
                if (argType.type == DoubleType && paramType == IntType) {
                    cerr << "warning: implicit conversion from 'double' to 'int' in argument "
                         << (i + 1) << " of call to '" << funcName << "'\n";
                }
                if (paramType == PointerType) {
                    bool samePtr = (argType.type == PointerType &&
                                    argType.pointeeType == functionParamPointeeTypes[funcName][i] &&
                                    argType.pointerDepth == functionParamPointerDepths[funcName][i]);
                    bool voidCompat = (argType.type == PointerType &&
                                       argType.pointerDepth == 1 && functionParamPointerDepths[funcName][i] == 1 &&
                                       (argType.pointeeType == VoidType || functionParamPointeeTypes[funcName][i] == VoidType));
                    bool ok = samePtr || voidCompat ||
                              (argType.type == IntType && argType.isZeroLiteral);
                    if (!ok) {
                        cerr << "error: incompatible pointer argument " << (i + 1) << " for call to '" << funcName << "'\n";
                        errorFlag = true;
                    }
                }
            }
        }
        if (functionReturnType.count(funcName)) {
            Type retType = functionReturnType[funcName];
            Type retPointee = functionReturnPointeeType.count(funcName) ? functionReturnPointeeType[funcName] : IntType;
            int retDepth = functionReturnPointerDepth.count(funcName) ? functionReturnPointerDepth[funcName] : 0;
            return {retType, false, retPointee, retDepth};
        }
        return {IntType, false, IntType, 0};
    }

    return {IntType, false, IntType, 0};
}
// --- Signature parsing helper ---

SymbolTableVisitor::FuncSignature SymbolTableVisitor::parseSignature(
    antlr4::tree::TerminalNode *typeNode, ifccParser::Ptr_suffixContext *returnPtrSuffix, antlr4::tree::TerminalNode *varNode,
        ifccParser::Param_listContext *paramList) {
    FuncSignature sig;
    sig.name = varNode->getText();

    string retTypeStr = typeNode->getText();
    sig.returnType = parseDeclaredType(retTypeStr, returnPtrSuffix, sig.returnPointeeType, sig.returnPointerDepth);

    sig.paramCount = 0;
    if (paramList) {
        auto types = paramList->TYPE();
        auto ptrSuffixes = paramList->ptr_suffix();
        sig.paramCount = types.size();
        for (size_t i = 0; i < types.size(); i++) {
            if (types[i]->getText() == "void" && (i >= ptrSuffixes.size() || ptrSuffixes[i]->getText().empty())) {
                cerr << "error: parameter has incomplete type 'void'\n";
                errorFlag = true;
                sig.paramTypes.push_back(IntType);
                sig.paramPointeeTypes.push_back(IntType);
                sig.paramPointerDepths.push_back(0);
                continue;
            }
            Type pointee = IntType;
            int pdepth = 0;
            Type pt = parseDeclaredType(types[i]->getText(), i < ptrSuffixes.size() ? ptrSuffixes[i] : nullptr, pointee, pdepth);
            sig.paramTypes.push_back(pt);
            sig.paramPointeeTypes.push_back(pointee);
            sig.paramPointerDepths.push_back(pdepth);
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
    FuncSignature sig = parseSignature(ctx->TYPE(), ctx->ptr_suffix(), ctx->VAR(), ctx->param_list());
    string funcName = sig.name;
    bool isPrototype = (ctx->block() == nullptr);

    if (isPrototype) {
        if (knownFunctions.count(funcName)) {
            bool conflict = false;
            if (functionReturnType[funcName] != sig.returnType) conflict = true;
            if (functionReturnPointeeType[funcName] != sig.returnPointeeType) conflict = true;
            if (functionReturnPointerDepth[funcName] != sig.returnPointerDepth) conflict = true;
            if (functionArgCount[funcName] != sig.paramCount) conflict = true;
            if (!conflict && functionParamTypes[funcName] != sig.paramTypes) conflict = true;
            if (!conflict && functionParamPointeeTypes[funcName] != sig.paramPointeeTypes) conflict = true;
            if (!conflict && functionParamPointerDepths[funcName] != sig.paramPointerDepths) conflict = true;

            if (conflict) {
                cerr << "error: conflicting declaration for function '" << funcName << "'\n";
                errorFlag = true;
            }
            return 0;
        }

        knownFunctions.insert(funcName);
        functionReturnType[funcName] = sig.returnType;
        functionReturnPointeeType[funcName] = sig.returnPointeeType;
        functionReturnPointerDepth[funcName] = sig.returnPointerDepth;
        functionArgCount[funcName] = sig.paramCount;
        functionParamTypes[funcName] = sig.paramTypes;
        functionParamPointeeTypes[funcName] = sig.paramPointeeTypes;
        functionParamPointerDepths[funcName] = sig.paramPointerDepths;
        return 0;
    }

    if (definedFunctions.count(funcName)) {
        cerr << "error: function '" << funcName << "' already defined\n";
        errorFlag = true;
    } else if (knownFunctions.count(funcName)) {
        bool conflict = false;
        if (functionReturnType[funcName] != sig.returnType) conflict = true;
        if (functionReturnPointeeType[funcName] != sig.returnPointeeType) conflict = true;
        if (functionReturnPointerDepth[funcName] != sig.returnPointerDepth) conflict = true;
        if (functionArgCount[funcName] != sig.paramCount) conflict = true;
        if (!conflict && functionParamTypes[funcName] != sig.paramTypes) conflict = true;
        if (!conflict && functionParamPointeeTypes[funcName] != sig.paramPointeeTypes) conflict = true;
        if (!conflict && functionParamPointerDepths[funcName] != sig.paramPointerDepths) conflict = true;

        if (conflict) {
            cerr << "error: conflicting types for function '" << funcName << "'\n";
            errorFlag = true;
        }
    }

    currentFunctionReturnType = sig.returnType;
    currentFunctionReturnPointeeType = sig.returnPointeeType;
    currentFunctionReturnPointerDepth = sig.returnPointerDepth;
    functionReturnType[funcName] = sig.returnType;
    functionReturnPointeeType[funcName] = sig.returnPointeeType;
    functionReturnPointerDepth[funcName] = sig.returnPointerDepth;
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
        functionParamPointeeTypes[funcName] = sig.paramPointeeTypes;
        functionParamPointerDepths[funcName] = sig.paramPointerDepths;

        enterScope();
        for (size_t i = 0; i < params.size(); i++) {
            declareVar(params[i]->getText(), sig.paramTypes[i], false, 0, sig.paramPointeeTypes[i], sig.paramPointerDepths[i]);
        }
    } else {
        functionArgCount[funcName] = 0;
        functionParamTypes[funcName] = {};
        functionParamPointeeTypes[funcName] = {};
        functionParamPointerDepths[funcName] = {};
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
    auto items = ctx->decl_item();
    auto ptrSuffixes = ctx->ptr_suffix();

    for (size_t i = 0; i < items.size(); i++) {
        if (ctx->TYPE()->getText() == "void") {
            cerr << "error: variable has incomplete type 'void'\n";
            errorFlag = true;
            continue;
        }

        // Chaque declarateur peut avoir son propre niveau d'indirection
        currentDeclType = parseDeclaredType(ctx->TYPE()->getText(), ptrSuffixes[i], currentDeclPointeeType, currentDeclPointerDepth);
        this->visit(items[i]);
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

    declareVar(varName, currentDeclType, isArray, arraySize, currentDeclPointeeType, currentDeclPointerDepth);

    if (ctx->expr()) {
        ExprTypeInfo exprType = inferExprType(ctx->expr());
        if (exprType.type == VoidType) {
            cerr << "error: void value not ignored as it ought to be\n";
            errorFlag = true;
        }
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) {
    // Semantique volontairement simple et proche de C : l'expression du switch doit etre int.
    ExprTypeInfo t = inferExprType(ctx->expr());
    if (t.type != IntType) {
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
            ExprTypeInfo ret = inferExprType(ctx->expr());
            if (ret.type == VoidType) {
                cerr << "error: void value not ignored as it ought to be\n";
                errorFlag = true;
            }
            if (currentFunctionReturnType == PointerType) {
                bool samePtr = (ret.type == PointerType &&
                                ret.pointeeType == currentFunctionReturnPointeeType &&
                                ret.pointerDepth == currentFunctionReturnPointerDepth);
                bool voidCompat = (ret.type == PointerType && ret.pointerDepth == 1 && currentFunctionReturnPointerDepth == 1 &&
                                   (ret.pointeeType == VoidType || currentFunctionReturnPointeeType == VoidType));
                bool ok = samePtr || voidCompat ||
                          (ret.type == IntType && ret.isZeroLiteral);
                if (!ok) {
                    cerr << "error: return type mismatch (expected pointer)\n";
                    errorFlag = true;
                }
            } else if (ret.type == PointerType) {
                cerr << "error: return type mismatch (cannot return pointer here)\n";
                errorFlag = true;
            }
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

antlrcpp::Any SymbolTableVisitor::visitDeref(ifccParser::DerefContext *ctx){
    inferExprType(ctx);

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAddressOf(ifccParser::AddressOfContext *ctx){
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
    enterScope();
    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
    }
    exitScope();
    return 0;
}

