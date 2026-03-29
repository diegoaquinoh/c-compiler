#include "IRGenVisitor.h"

using namespace std;

static const string ireg = "!reg";
static const string freg = "!freg";

static int getCaseValue(ifccParser::Case_valueContext *ctx) {
    return stoi(ctx->getText());
}

string IRGenVisitor::activeReg(Type t) const {
    // On reserve !reg aux entiers et !freg aux doubles.
    return (t == DoubleType) ? freg : ireg;
}

Type IRGenVisitor::inferExprType(ifccParser::ExprContext *ctx) {
    if (ctx == nullptr) {
        return IntType;
    }

    if (auto *c = dynamic_cast<ifccParser::ConstContext *>(ctx)) {
        return c->DOUBLE_CONST() ? DoubleType : IntType;
    }

    if (auto *f = dynamic_cast<ifccParser::FuncCallContext *>(ctx)) {
        string funcName = f->VAR()->getText();
        if (functionReturnType.count(funcName))
            return functionReturnType[funcName];
        return IntType;
    }

    if (auto *v = dynamic_cast<ifccParser::VarContext *>(ctx)) {
        string irName = scopedName(v->VAR()->getText());
        return this->ir.currentCfg->get_var_type(irName);
    }

    if (auto *p = dynamic_cast<ifccParser::ParensContext *>(ctx)) {
        return inferExprType(p->expr());
    }

    if (auto *n = dynamic_cast<ifccParser::NegativeContext *>(ctx)) {
        return inferExprType(n->expr());
    }

    if (dynamic_cast<ifccParser::LogicalnotContext *>(ctx)) {
        return IntType;
    }

    if (auto *m = dynamic_cast<ifccParser::MultdivContext *>(ctx)) {
        if (m->OP->getText() == "%") {
            return IntType;
        }
        Type lhs = inferExprType(m->expr(0));
        Type rhs = inferExprType(m->expr(1));
        return (lhs == DoubleType || rhs == DoubleType) ? DoubleType : IntType;
    }

    if (auto *a = dynamic_cast<ifccParser::AddsubContext *>(ctx)) {
        Type lhs = inferExprType(a->expr(0));
        Type rhs = inferExprType(a->expr(1));
        return (lhs == DoubleType || rhs == DoubleType) ? DoubleType : IntType;
    }

    if (dynamic_cast<ifccParser::RelationalContext *>(ctx) ||
        dynamic_cast<ifccParser::EqualityContext *>(ctx) ||
        dynamic_cast<ifccParser::BitwiseandContext *>(ctx) ||
        dynamic_cast<ifccParser::BitwisexorContext *>(ctx) ||
        dynamic_cast<ifccParser::BitwiseorContext *>(ctx)) {
        return IntType;
    }

    if (auto *a = dynamic_cast<ifccParser::AffectStmtContext *>(ctx)) {
        return inferExprType(a->expr(0));
    }

    if (auto *a = dynamic_cast<ifccParser::ArrayAccessContext *>(ctx)) {
        string irName = scopedName(a->VAR()->getText());
        return this->ir.currentCfg->get_var_type(irName);
    }

    return IntType;
}

void IRGenVisitor::emitConvert(Type src, Type dst, const string &srcName, const string &dstName) {
    // Conversion explicite au niveau IR quand les types source/destination different.
    if (src == dst) {
        if (srcName != dstName) {
            vector<string> copy = {dstName, srcName};
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, src, copy);
        }
        return;
    }

    if (src == IntType && dst == DoubleType) {
        vector<string> conv = {dstName, srcName};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::itod, DoubleType, conv);
        return;
    }

    if (src == DoubleType && dst == IntType) {
        // Toujours vraie cette condition ?
        vector<string> conv = {dstName, srcName};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::dtoi, IntType, conv);
    }
}

void IRGenVisitor::ensureValueInReg(Type currentType, Type targetType) {
    // la valeur courante est placee dans le registre correspondant a targetType.
    emitConvert(currentType, targetType, activeReg(currentType), activeReg(targetType));
}

string IRGenVisitor::createVariableTmp(Type t) {
    string nameVar = "!tmp" + to_string(cptTempVariables++);
    this->ir.currentCfg->add_to_symbol_table(nameVar, t);
    return nameVar;
}

string IRGenVisitor::emitArrayElementOffset(const string &arrayScopedName, ifccParser::ExprContext *indexExpr) {
    // Etape 1: evaluer l'indice dans l'accumulateur (!reg)
    Type indexType = inferExprType(indexExpr);
    this->visit(indexExpr);
    ensureValueInReg(indexType, IntType);

    string indexTmp = createVariableTmp(IntType);
    vector<string> saveIndex = {indexTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, saveIndex);

    string elemSizeTmp = createVariableTmp(IntType);
    vector<string> ldElemSize = {elemSizeTmp, "8"};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, ldElemSize);

    // Etape 2: offset element = index * tailleCase
    string scaledIndexTmp = createVariableTmp(IntType);
    vector<string> scaleIndex = {scaledIndexTmp, indexTmp, elemSizeTmp};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::mul, IntType, scaleIndex);

    string baseOffsetTmp = createVariableTmp(IntType);
    int baseOffset = this->ir.currentCfg->get_var_frame_offset(arrayScopedName);
    vector<string> ldBaseOffset = {baseOffsetTmp, to_string(baseOffset)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, ldBaseOffset);

    // Etape 3: offset final relatif a %rbp
    // La pile croit vers les adresses basses, l'element i est a (base - i * tailleCase)
    string finalOffsetTmp = createVariableTmp(IntType);
    vector<string> sumOffset = {finalOffsetTmp, baseOffsetTmp, scaledIndexTmp};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::sub, IntType, sumOffset);

    return finalOffsetTmp;
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
    for (auto *func : ctx->func()) {
        this->visit(func);
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitFunc(ifccParser::FuncContext *ctx)
{
    string funcName = ctx->VAR()->getText();

    string retTypeStr = ctx->TYPE()->getText();
    if (retTypeStr == "double") currentFunctionReturnType = DoubleType;
    else if (retTypeStr == "void") currentFunctionReturnType = VoidType;
    else currentFunctionReturnType = IntType;
    functionReturnType[funcName] = currentFunctionReturnType;

    vector<Type> paramTypes;
    if (ctx->param_list()) {
        auto *paramList = ctx->param_list();
        auto types = paramList->TYPE();
        for (size_t i = 0; i < types.size(); i++) {
            Type pt = (types[i]->getText() == "double") ? DoubleType : IntType;
            paramTypes.push_back(pt);
        }
    }
    if (ctx->block() == nullptr) {
        functionParamTypes[funcName] = paramTypes;
        return 0;
    }

    // Full function definition — create CFG and generate IR
    CFG* cfg = new CFG();
    cfg->functionName = funcName;

    // Reset scope state
    scopeStack.clear();
    scopeCounter = 0;
    arraySizeByScopedName.clear();
    enterScope(); // function-level scope

    // Register parameter names and types
    if (ctx->param_list()) {
        auto params = ctx->param_list()->VAR();
        auto types = ctx->param_list()->TYPE();
        vector<Type> definitionParamTypes;
        for (size_t i = 0; i < params.size(); i++) {
            string paramName = params[i]->getText();
            string irName = declareScoped(paramName);
            cfg->paramNames.push_back(irName);
            Type paramType = (types[i]->getText() == "double") ? DoubleType : IntType;
            definitionParamTypes.push_back(paramType);
            cfg->add_to_symbol_table(irName, paramType);
        }
        functionParamTypes[funcName] = definitionParamTypes;
    } else {
        functionParamTypes[funcName] = {};
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
    breakTriggered = false;

    // Visit the function body
    this->visit(ctx->block());

    // Implicit return 0 for main (or any int function without explicit return)
    if (!cfg->current_bb->has_return) {
        string retType = ctx->TYPE()->getText();
        if (retType == "int") {
            vector<string> loadZero = {ireg, "0"};
            cfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, loadZero);

            vector<string> retZero = {ireg};
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
    if (ctx->TYPE()->getText() == "void") return 0;
    currentDeclType = (ctx->TYPE()->getText() == "double") ? DoubleType : IntType;
    for (auto *item : ctx->decl_item()) {
        this->visit(item);
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitDecl_item(ifccParser::Decl_itemContext *ctx)
{
    string varName = ctx->VAR()->getText();
    string irName = declareScoped(varName);
    this->ir.currentCfg->add_to_symbol_table(irName, currentDeclType);

    if (ctx->CONST()) {
        // Alors c'est la déclaration d'un tableau
        int arraySize = stoi(ctx->CONST()->getText());
        arraySizeByScopedName[irName] = arraySize;

        // On reserve des cases contigues pour que les acces par offset soient lineaires en memoire
        // Convention de nom : nomTab#indice
        for (int i = 1; i < arraySize; i++) {
            this->ir.currentCfg->add_to_symbol_table(irName + "#" + to_string(i), currentDeclType);
        }
    }

    if (ctx->expr()) {
        // Initialisation: evaluer l'expression, convertir si besoin, puis copier vers la variable.
        Type exprType = inferExprType(ctx->expr());
        this->visit(ctx->expr());
        ensureValueInReg(exprType, currentDeclType);
        vector<string> v = {irName, activeReg(currentDeclType)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, currentDeclType, v);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAffectStmt(ifccParser::AffectStmtContext *ctx)
{
    auto *lhsArray = dynamic_cast<ifccParser::ArrayAccessContext *>(ctx->expr(0));

    if (lhsArray != nullptr) {
        // Tableau
        string arrayScopedName = scopedName(lhsArray->VAR()->getText());
        Type elemType = this->ir.currentCfg->get_var_type(arrayScopedName);

        // On evalue d'abord la RHS, puis on la fige dans un temporaire pour ne pas la perdre
        // pendant le calcul de l'adresse mémoire
        Type rhsType = inferExprType(ctx->expr(1));
        this->visit(ctx->expr(1));
        ensureValueInReg(rhsType, elemType);

        string rhsTmp = createVariableTmp(elemType);
        vector<string> saveRhs = {rhsTmp, activeReg(elemType)};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, elemType, saveRhs);

        string lvalueOffset = emitArrayElementOffset(arrayScopedName, lhsArray->expr());
        vector<string> store = {lvalueOffset, rhsTmp};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::wmem, elemType, store);

        // On fixe la valeur affectee dans l'accumulateur pour la renvoyer
        vector<string> restoreExprVal = {activeReg(elemType), rhsTmp};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, elemType, restoreExprVal);
        return 0;
    }

    auto *lhsVar = dynamic_cast<ifccParser::VarContext *>(ctx->expr(0));
    // Variable
    string varName = lhsVar->VAR()->getText();
    string irName = scopedName(varName);
    Type varType = this->ir.currentCfg->get_var_type(irName);

    // le type cible est celui de la lvalue de gauche
    Type exprType = inferExprType(ctx->expr(1));
    this->visit(ctx->expr(1));
    ensureValueInReg(exprType, varType);

    // On sauvegarde la valeur de droite avant de calculer l'adresse et offset de la lvalue
    // sinon on va l'écraser
    string rhsTmp = createVariableTmp(varType);
    vector<string> saveRhs = {rhsTmp, activeReg(varType)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, varType, saveRhs);

    // Lvalue simple pour une variable : offset relatif a %rbp
    string lvalueOffset = createVariableTmp(IntType);
    int targetOffset = this->ir.currentCfg->get_var_frame_offset(irName);
    vector<string> ldOffset = {lvalueOffset, to_string(targetOffset)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, ldOffset);

    vector<string> store = {lvalueOffset, rhsTmp};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::wmem, varType, store);

    // On met la valeur dans le registre accumulateur car une affectation doit renvoyer la valeur
    vector<string> restoreExprVal = {activeReg(varType), rhsTmp};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, varType, restoreExprVal);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitConst(ifccParser::ConstContext *ctx)
{
    if (ctx->DOUBLE_CONST()) {
        // Les constantes double sont chargees dans !freg
        vector<string> v = {freg, ctx->DOUBLE_CONST()->getText()};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, DoubleType, v);
        return 0;
    }

    int val;
    if (ctx->CONST()) {
        val = stoi(ctx->CONST()->getText());
    } else {
        string token = ctx->CHAR_CONST()->getText();
        if (token[1] == '\\') {
            switch (token[2]) {
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

    vector<string> v = {ireg, to_string(val)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, IntType, v);
    return 0;
}

antlrcpp::Any IRGenVisitor::visitVar(ifccParser::VarContext *ctx)
{
    string varName = ctx->VAR()->getText();
    string irName = scopedName(varName);
    Type varType = this->ir.currentCfg->get_var_type(irName);

    vector<string> v = {activeReg(varType), irName};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, varType, v);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitArrayAccess(ifccParser::ArrayAccessContext *ctx)
{
    string varName = ctx->VAR()->getText();
    string arrayScopedName = scopedName(varName);
    Type elemType = this->ir.currentCfg->get_var_type(arrayScopedName);

    // rmem lit *(rbp + offset) dans le registre actif du type
    string elementOffset = emitArrayElementOffset(arrayScopedName, ctx->expr());
    vector<string> load = {activeReg(elemType), elementOffset};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::rmem, elemType, load);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    CFG *cfg = this->ir.currentCfg;
    BasicBlock *testBB = cfg->current_bb;
    BasicBlock *trueBB = new BasicBlock(cfg, cfg->new_BB_name() + "_true");
    BasicBlock *endBB = new BasicBlock(cfg, cfg->new_BB_name() + "_endif");
    BasicBlock *elseBB = nullptr;

    if (ctx->else_stmt()) {
        elseBB = new BasicBlock(cfg, cfg->new_BB_name() + "_else");
    }

    Type condType = inferExprType(ctx->expr());
    // Evaluate the condition in the current block
    this->visit(ctx->expr());
    if (condType == DoubleType) {
        // if attend un booleen entier (sot 0 soit 1 donc) 
        // Si on a un double, on genere (expr != 0.0) (comme si on "castait" la condition en double)
        string zeroTmp = createVariableTmp(DoubleType);
        vector<string> z = {zeroTmp, "0.0"};
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, DoubleType, z);
        vector<string> cmp = {ireg, freg, zeroTmp};
        cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, DoubleType, cmp);
    }
    testBB->test_var_name = ireg;

    // Link testBB
    testBB->exit_true = trueBB;
    testBB->exit_false = elseBB ? elseBB : endBB;

    // Build true branch
    cfg->add_bb(trueBB);
    cfg->current_bb = trueBB;
    this->visit(ctx->block());
    if (!cfg->current_bb->has_return) {
        cfg->current_bb->exit_true = endBB;
        cfg->current_bb->exit_false = nullptr;
    }

    // Build else branch if it exists
    if (elseBB) {
        cfg->add_bb(elseBB);
        cfg->current_bb = elseBB;
        this->visit(ctx->else_stmt());
        if (!cfg->current_bb->has_return) {
            cfg->current_bb->exit_true = endBB;
            cfg->current_bb->exit_false = nullptr;
        }
    }

    // Continue building in the endBB
    cfg->add_bb(endBB);
    cfg->current_bb = endBB;

    return 0;
}

antlrcpp::Any IRGenVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx)
{
    CFG *cfg = this->ir.currentCfg;
    BasicBlock *entryBB = cfg->current_bb;
    BasicBlock *afterSwitch = new BasicBlock(cfg, cfg->new_BB_name() + "_switch_after");

    this->breakTriggered = false;

    this->visit(ctx->expr());
    string switchValue = createVariableTmp(IntType);
    vector<string> saveSwitchValue = {switchValue, ireg};
    entryBB->add_IRInstr(IRInstr::copy, IntType, saveSwitchValue);

    auto clauses = ctx->switch_clause();
    enterScope();
    if (clauses.empty()) {
        entryBB->exit_true = afterSwitch;
        entryBB->exit_false = nullptr;
        exitScope();
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

        string caseValueVar = createVariableTmp(IntType);
        vector<string> loadCaseValue = {
            caseValueVar,
            to_string(getCaseValue(clauses[caseClauseIndexes[i]]->case_label()->case_value()))
        };
        testBB->add_IRInstr(IRInstr::ldconst, IntType, loadCaseValue);

        vector<string> compareCase = {ireg, switchValue, caseValueVar};
        testBB->add_IRInstr(IRInstr::cmp_eq, IntType, compareCase);
        testBB->test_var_name = ireg;
        testBB->test_var_name = ireg;
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
    exitScope();
    cfg->add_bb(afterSwitch);
    cfg->current_bb = afterSwitch;

    return 0;
}

antlrcpp::Any IRGenVisitor::visitElse_stmt(ifccParser::Else_stmtContext *ctx)
{
    this->visit(ctx->block());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) 
{
    CFG *cfg = this->ir.currentCfg;

    // Create necessary basic blocks
    BasicBlock *condBB = new BasicBlock(cfg, cfg->new_BB_name() + "_cond");
    BasicBlock *bodyBB = new BasicBlock(cfg, cfg->new_BB_name() + "_body");
    BasicBlock *endBB = new BasicBlock(cfg, cfg->new_BB_name() + "_endwhile");

    // Connect the current block to the condition block
    cfg->current_bb->exit_true = condBB;
    cfg->current_bb->exit_false = nullptr;

    // Build condition block
    cfg->add_bb(condBB);
    cfg->current_bb = condBB;

    Type condType = inferExprType(ctx->expr());
    this->visit(ctx->expr());
    if (condType == DoubleType) {
        // Meme principe que pour if "caster" la condition double en entier via != 0.0.
        string zeroTmp = createVariableTmp(DoubleType);
        vector<string> z = {zeroTmp, "0.0"};
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, DoubleType, z);
        vector<string> cmp = {ireg, freg, zeroTmp};
        cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, DoubleType, cmp);
    }
    condBB->test_var_name = ireg;
    // Condition block exits
    condBB->exit_true = bodyBB;
    condBB->exit_false = endBB;

    cfg->push_break_target(endBB);

    // Build body block
    cfg->add_bb(bodyBB);
    cfg->current_bb = bodyBB;
    enterScope();
    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
        if (cfg->current_bb->has_return || breakTriggered) {
            break;
        }
    }
    exitScope();
    
    // Loop back to condition
    if (breakTriggered) {
        breakTriggered = false;
    } else if (!cfg->current_bb->has_return) {
        cfg->current_bb->exit_true = condBB;
        cfg->current_bb->exit_false = nullptr;
    }

    cfg->pop_break_target();

    // Continue building in the endBB
    cfg->add_bb(endBB);
    cfg->current_bb = endBB;

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
    if (ctx->expr()) {
        Type exprType = inferExprType(ctx->expr());
        this->visit(ctx->expr());

        ensureValueInReg(exprType, IntType);

        vector<string> v = {ireg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::rtrn, IntType, v);
    }
    // else: bare "return;" -- no rtrn instruction, just trigger epilogue

    this->ir.currentCfg->current_bb->has_return = true;
    this->ir.currentCfg->current_bb->exit_true = nullptr;
    this->ir.currentCfg->current_bb->exit_false = nullptr;
    return 0;
}

antlrcpp::Any IRGenVisitor::visitMultdiv(ifccParser::MultdivContext *ctx)
{
    auto op = ctx->OP->getText();

    Type lhsType = inferExprType(ctx->expr(0));
    Type rhsType = inferExprType(ctx->expr(1));
    // Modulo ne fonctionne qu'avec des entiers, div et mult peu importe on peut caster
    Type resultType = (op == "%") ? IntType : ((lhsType == DoubleType || rhsType == DoubleType) ? DoubleType : IntType);

    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(lhsType);
    vector<string> saveLhs = {lhsTmp, activeReg(lhsType)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, saveLhs);

    this->visit(ctx->expr(1));

    string lhsOp = lhsTmp;
    string rhsOp = activeReg(rhsType);

    if (resultType == DoubleType) {
        // Si on veut un résultat double, il convertir chacune des opérandes en double

        // Si on fait la conversion de lhs juste après, on va écraser la valeur de freg
        if (rhsType == DoubleType && lhsType == IntType) {
            string rhsTmp = createVariableTmp(DoubleType);
            vector<string> saveRhs = {rhsTmp, freg};
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, DoubleType, saveRhs);
            rhsOp = rhsTmp;
        }

        if (lhsType == IntType) {
            string lhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, lhsTmp, lhsDouble);
            lhsOp = lhsDouble;
        }
        if (rhsType == IntType) {
            string rhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, ireg, rhsDouble);
            rhsOp = rhsDouble;
        }

        vector<string> args = {freg, lhsOp, rhsOp};
        if (op == "*") {
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::mul, DoubleType, args);
        } else if (op == "/") {
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::div, DoubleType, args);
        }
        return 0;
    }

    // On attend un résultat entier
    vector<string> args = {ireg, lhsOp, rhsOp};
    if (op == "*") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::mul, IntType, args);
    } else if (op == "/") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::div, IntType, args);
    } else if (op == "%") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::mod, IntType, args);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitAddsub(ifccParser::AddsubContext *ctx)
{  
    // Pareil que pour mult, div et modulo : 
    // En fonction du typedes opérandes on détermine le type résultat 
    auto op = ctx->OP->getText();

    Type lhsType = inferExprType(ctx->expr(0));
    Type rhsType = inferExprType(ctx->expr(1));
    Type resultType = (lhsType == DoubleType || rhsType == DoubleType) ? DoubleType : IntType;

    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(lhsType);
    vector<string> saveLhs = {lhsTmp, activeReg(lhsType)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, saveLhs);

    this->visit(ctx->expr(1));

    string lhsOp = lhsTmp;
    string rhsOp = activeReg(rhsType);

    if (resultType == DoubleType) {
        // Si on fait la conversion de lhs juste après, on va écraser la valeur de freg
        if (rhsType == DoubleType && lhsType == IntType) {
            string rhsTmp = createVariableTmp(DoubleType);
            vector<string> saveRhs = {rhsTmp, freg};
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, DoubleType, saveRhs);
            rhsOp = rhsTmp;
        }

        if (lhsType == IntType) {
            string lhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, lhsTmp, lhsDouble);
            lhsOp = lhsDouble;
        }
        if (rhsType == IntType) {
            string rhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, ireg, rhsDouble);
            rhsOp = rhsDouble;
        }

        vector<string> args = {freg, lhsOp, rhsOp};
        if (op == "+") {
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::add, DoubleType, args);
        } else {
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::sub, DoubleType, args);
        }
        return 0;
    }

    vector<string> args = {ireg, lhsOp, rhsOp};
    if (op == "+") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::add, IntType, args);
    } else {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::sub, IntType, args);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitNegative(ifccParser::NegativeContext *ctx) {
    Type exprType = inferExprType(ctx->expr());
    this->visit(ctx->expr());
    vector<string> v = {activeReg(exprType), activeReg(exprType)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::neg, exprType, v);
    return 0;
}

antlrcpp::Any IRGenVisitor::visitParens(ifccParser::ParensContext *ctx){
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseand(ifccParser::BitwiseandContext *ctx){
    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(IntType);
    vector<string> saveLhs = {lhsTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, saveLhs);

    this->visit(ctx->expr(1));
    vector<string> args = {ireg, lhsTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::band, IntType, args);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwisexor(ifccParser::BitwisexorContext *ctx){
    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(IntType);
    vector<string> saveLhs = {lhsTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, saveLhs);

    this->visit(ctx->expr(1));
    vector<string> args = {ireg, lhsTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::bxor, IntType, args);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitBitwiseor(ifccParser::BitwiseorContext *ctx){

    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(IntType);
    vector<string> saveLhs = {lhsTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, IntType, saveLhs);

    this->visit(ctx->expr(1));
    vector<string> args = {ireg, lhsTmp, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::bor, IntType, args);

    return 0;
}

antlrcpp::Any IRGenVisitor::visitFuncCall(ifccParser::FuncCallContext *ctx) {
    string funcName = ctx->VAR()->getText();
    auto args = ctx->expr();

    // 1. Evaluate each arg and convert to the expected parameter type
    vector<string> argTempNames;
    for (size_t i = 0; i < args.size(); i++) {
        Type argType = inferExprType(args[i]);
        this->visit(args[i]);

        // Determine target type from function signature
        Type targetType = IntType;
        if (functionParamTypes.count(funcName) && i < functionParamTypes[funcName].size()) {
            targetType = functionParamTypes[funcName][i];
        }

        ensureValueInReg(argType, targetType);

        string tempName = createVariableTmp(targetType);
        string reg = activeReg(targetType);
        vector<string> copyArg = {tempName, reg};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, targetType, copyArg);
        argTempNames.push_back(tempName);
    }

    // 2. Generate call instruction with the function name and the temp stack slots as arguments
    vector<string> callArgs = {ireg, funcName};
    callArgs.insert(callArgs.end(), argTempNames.begin(), argTempNames.end());
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::call, IntType, callArgs);
    return 0;
}

antlrcpp::Any IRGenVisitor::visitRelational(ifccParser::RelationalContext *ctx){
    auto op = ctx->OP->getText();

    Type lhsType = inferExprType(ctx->expr(0));
    Type rhsType = inferExprType(ctx->expr(1));
    // Les comparaisons prennent un type de calcul (soit int soit double) 
    // et renvoient un int (0 ou 1)
    Type cmpType = (lhsType == DoubleType || rhsType == DoubleType) ? DoubleType : IntType;

    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(lhsType);
    vector<string> saveLhs = {lhsTmp, activeReg(lhsType)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, saveLhs);

    this->visit(ctx->expr(1));

    string lhsOp = lhsTmp;
    string rhsOp = activeReg(rhsType);

    if (cmpType == DoubleType) {
        // Si on fait la conversion de lhs juste après, on va écraser la valeur de freg
        if (rhsType == DoubleType && lhsType == IntType) {
            string rhsTmp = createVariableTmp(DoubleType);
            vector<string> saveRhs = {rhsTmp, freg};
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, DoubleType, saveRhs);
            rhsOp = rhsTmp;
        }
        
        if (lhsType == IntType) {
            string lhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, lhsTmp, lhsDouble);
            lhsOp = lhsDouble;
        }
        if (rhsType == IntType) {
            string rhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, ireg, rhsDouble);
            rhsOp = rhsDouble;
        }
    }

    vector<string> cmpArgs = {ireg, lhsOp, rhsOp};
    if (op == "<") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_lt, cmpType, cmpArgs);
    } else if (op == "<=") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_le, cmpType, cmpArgs);
    } else if (op == ">") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_gt, cmpType, cmpArgs);
    } else if (op == ">=") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_ge, cmpType, cmpArgs);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitEquality(ifccParser::EqualityContext *ctx){
    auto op = ctx->OP->getText();

    Type lhsType = inferExprType(ctx->expr(0));
    Type rhsType = inferExprType(ctx->expr(1));
    // Les comparaisons prennent un type de calcul (int/double) et retournent booleen
    Type cmpType = (lhsType == DoubleType || rhsType == DoubleType) ? DoubleType : IntType;

    this->visit(ctx->expr(0));
    string lhsTmp = createVariableTmp(lhsType);
    vector<string> saveLhs = {lhsTmp, activeReg(lhsType)};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, lhsType, saveLhs);

    this->visit(ctx->expr(1));

    string lhsOp = lhsTmp;
    string rhsOp = activeReg(rhsType);

    if (cmpType == DoubleType) {
        // Si on fait la conversion de lhs juste après, on va écraser la valeur de freg
        if (rhsType == DoubleType && lhsType == IntType) {
            string rhsTmp = createVariableTmp(DoubleType);
            vector<string> saveRhs = {rhsTmp, freg};
            this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::copy, DoubleType, saveRhs);
            rhsOp = rhsTmp;
        }
        if (lhsType == IntType) {
            string lhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, lhsTmp, lhsDouble);
            lhsOp = lhsDouble;
        }
        if (rhsType == IntType) {
            string rhsDouble = createVariableTmp(DoubleType);
            emitConvert(IntType, DoubleType, ireg, rhsDouble);
            rhsOp = rhsDouble;
        }
    }

    vector<string> cmpArgs = {ireg, lhsOp, rhsOp};
    if (op == "==") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_eq, cmpType, cmpArgs);
    } else if (op == "!=") {
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_ne, cmpType, cmpArgs);
    }

    return 0;
}

antlrcpp::Any IRGenVisitor::visitLogicalnot(ifccParser::LogicalnotContext *ctx) {
    Type exprType = inferExprType(ctx->expr());
    this->visit(ctx->expr());

    if (exprType == DoubleType) {
        // On convertit d'abord en entier via (x != 0.0), puis on peut faire le lnot
        string zeroTmp = createVariableTmp(DoubleType);
        vector<string> z = {zeroTmp, "0.0"};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::ldconst, DoubleType, z);

        vector<string> cmp = {ireg, freg, zeroTmp};
        this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::cmp_ne, DoubleType, cmp);
    }

    vector<string> v = {ireg, ireg};
    this->ir.currentCfg->current_bb->add_IRInstr(IRInstr::lnot, IntType, v);

    return 0;
}
