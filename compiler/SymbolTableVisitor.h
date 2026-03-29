#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "Type.h"
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

class SymbolTableVisitor: public ifccBaseVisitor {
    public:

        using ifccBaseVisitor::visit;

        // Scope management
        void enterScope();
        void exitScope();

        // Variable management
        void declareVar(const std::string &name, Type t, bool isArray = false, int arraySize = 0, Type pointeeType = IntType, int pointerDepth = 0);
        void useVar(const std::string &name);
        Type getVarType(const string &name) const;
        Type getVarPointeeType(const string &name) const;
        int getVarPointerDepth(const string &name) const;
        bool isVarArray(const string &name) const;

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitFunc(ifccParser::FuncContext *ctx) override;
        virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override ;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override ;

        virtual antlrcpp::Any visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) override;
        virtual antlrcpp::Any visitSwitch_clause(ifccParser::Switch_clauseContext *ctx) override;
        virtual antlrcpp::Any visitCase_label(ifccParser::Case_labelContext *ctx) override;
        virtual antlrcpp::Any visitDefault_label(ifccParser::Default_labelContext *ctx) override;
        virtual antlrcpp::Any visitCase_value(ifccParser::Case_valueContext *ctx) override;
        virtual antlrcpp::Any visitBreak_stmt(ifccParser::Break_stmtContext *ctx) override;

        virtual antlrcpp::Any visitAffectStmt(ifccParser::AffectStmtContext *ctx) override ;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override ;
        
        virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;

        virtual antlrcpp::Any visitElse_stmt(ifccParser::Else_stmtContext *ctx) override;

        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        virtual antlrcpp::Any visitNegative(ifccParser::NegativeContext *ctx) override;
        virtual antlrcpp::Any visitLogicalnot(ifccParser::LogicalnotContext *ctx) override;
        virtual antlrcpp::Any visitDeref(ifccParser::DerefContext *ctx) override;
        virtual antlrcpp::Any visitAddressOf(ifccParser::AddressOfContext *ctx) override;
        virtual antlrcpp::Any visitParens(ifccParser::ParensContext *ctx) override;

        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;
        virtual antlrcpp::Any visitRelational(ifccParser::RelationalContext *ctx) override;
        virtual antlrcpp::Any visitEquality(ifccParser::EqualityContext *ctx) override;

        virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override;
        virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;

        virtual antlrcpp::Any visitBitwiseand(ifccParser::BitwiseandContext *ctx) override;
        virtual antlrcpp::Any visitBitwisexor(ifccParser::BitwisexorContext *ctx) override;
        virtual antlrcpp::Any visitBitwiseor(ifccParser::BitwiseorContext *ctx) override;

        map<string, map<string, Type>> getAllSymbolTables() const { return allSymbolTables; }
        map<string, int> getFunctionArgCount() const { return functionArgCount; }
        map<string, Type> getFunctionReturnType() const { return functionReturnType; }
        map<string, vector<Type>> getFunctionParamTypes() const { return functionParamTypes; }
        virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

        bool hasError() const { return errorFlag; }

    private:
        struct VarInfo {
            string name;
            Type type;
            bool isArray;
            int arraySize;
            Type pointeeType;
            int pointerDepth;
        };

        struct ExprTypeInfo {
            Type type;
            bool isZeroLiteral;// Il faut savoir si un pointeur est nul (équivalent NULL) ou pas
            Type pointeeType;
            int pointerDepth;
        };

        const VarInfo *lookupVar(const string &name) const;
        bool isLvalueExpr(ifccParser::ExprContext *ctx) const;
        Type parseBaseType(const string &typeText) const;
        Type parseDeclaredType(const string &typeText, ifccParser::Ptr_suffixContext *ptrSuffix, Type &outPointeeType, int &outPointerDepth) const;
        int getPointedElementSize(const ExprTypeInfo &ptrType) const;
        bool isZeroLiteralExpr(ifccParser::ExprContext *ctx) const;

        struct FuncSignature {
            string name;
            Type returnType;
            Type returnPointeeType;
            int returnPointerDepth;
            int paramCount;
            vector<Type> paramTypes;
            vector<Type> paramPointeeTypes;
            vector<int> paramPointerDepths;
        };
        FuncSignature parseSignature(antlr4::tree::TerminalNode *typeNode, ifccParser::Ptr_suffixContext *returnPtrSuffix, antlr4::tree::TerminalNode *varNode, ifccParser::Param_listContext *paramList);
        ExprTypeInfo inferExprType(ifccParser::ExprContext *ctx);
        Type currentDeclType = IntType;
        Type currentDeclPointeeType = IntType;
        int currentDeclPointerDepth = 0;
        // Per-function scope stack
        vector<VarInfo> varStack;
        vector<int> scopeMarkers;              // marks where each scope begins in varStack

        // Global function registry
        string currentFunction;
        map<string, map<string, Type>> allSymbolTables;  // funcName -> (varName -> Type), built at end of each function
        set<string> usedVars;
        int nextIndex = 0;
        bool errorFlag = false;
        set<string> definedFunctions;
        set<string> knownFunctions = {"putchar", "getchar"};
        map<string, int> functionArgCount = {{"putchar", 1}, {"getchar", 0}};
        map<string, Type> functionReturnType = {{"putchar", IntType}, {"getchar", IntType}};
        map<string, Type> functionReturnPointeeType = {{"putchar", IntType}, {"getchar", IntType}};
        map<string, int> functionReturnPointerDepth = {{"putchar", 0}, {"getchar", 0}};
        map<string, vector<Type>> functionParamTypes = {{"putchar", {IntType}}, {"getchar", {}}};
        map<string, vector<Type>> functionParamPointeeTypes = {{"putchar", {IntType}}, {"getchar", {}}};
        map<string, vector<int>> functionParamPointerDepths = {{"putchar", {0}}, {"getchar", {}}};
        Type currentFunctionReturnType = IntType;
        Type currentFunctionReturnPointeeType = IntType;
        int currentFunctionReturnPointerDepth = 0;
};
