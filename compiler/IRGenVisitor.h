#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "IR.h"
#include <map>
#include <string>
#include <vector>
using namespace std;


class IRGenVisitor : public ifccBaseVisitor {
    public:
            using ifccBaseVisitor::visit;
	        IRGenVisitor() {}

        virtual string createVariableTmp(Type t = IntType);

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
        virtual antlrcpp::Any visitFunc(ifccParser::FuncContext *ctx) override;
        virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

        // Statement visitors
        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override;

        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        virtual antlrcpp::Any visitAffectStmt(ifccParser::AffectStmtContext *ctx) override;

        virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
        virtual antlrcpp::Any visitElse_stmt(ifccParser::Else_stmtContext *ctx) override;
        virtual antlrcpp::Any visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) override;
        virtual antlrcpp::Any visitBreak_stmt(ifccParser::Break_stmtContext *ctx) override;
        // Expression visitors

        virtual antlrcpp::Any visitNegative(ifccParser::NegativeContext *ctx) override;
        virtual antlrcpp::Any visitDeref(ifccParser::DerefContext *ctx) override;
        virtual antlrcpp::Any visitAddressOf(ifccParser::AddressOfContext *ctx) override;
        virtual antlrcpp::Any visitParens(ifccParser::ParensContext *ctx) override;

        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;

        virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override;
        virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;
        virtual antlrcpp::Any visitArrayAccess(ifccParser::ArrayAccessContext *ctx) override;

        virtual antlrcpp::Any visitBitwiseand(ifccParser::BitwiseandContext *ctx) override;
        virtual antlrcpp::Any visitBitwisexor(ifccParser::BitwisexorContext *ctx) override;
        virtual antlrcpp::Any visitBitwiseor(ifccParser::BitwiseorContext *ctx) override;

        virtual antlrcpp::Any visitRelational(ifccParser::RelationalContext *ctx) override;
        virtual antlrcpp::Any visitEquality(ifccParser::EqualityContext *ctx) override;

        virtual antlrcpp::Any visitLogicalnot(ifccParser::LogicalnotContext *ctx) override;

        virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

        // Scope management for variable renaming
        void enterScope();
        void exitScope();
        string scopedName(const string &name);       // lookup current scoped name
        string declareScoped(const string &name);     // declare and return scoped name

        IR& getIR() { return this->ir; }

        private:
                Type inferExprType(ifccParser::ExprContext *ctx);
                bool inferPointerInfo(ifccParser::ExprContext *ctx, Type &outBaseType, int &outDepth);
                int pointerElementSize(Type baseType, int depth) const;
                string activeReg(Type t) const;
                void emitConvert(Type src, Type dst, const string &srcName, const string &dstName);
                void ensureValueInReg(Type currentType, Type targetType);
                string emitArrayElementOffset(const string &arrayScopedName, ifccParser::ExprContext *indexExpr);
                string emitScaledPointerOffset(ifccParser::ExprContext *indexExpr, int elemSize);
                Type parseDeclaredType(const string &typeText, ifccParser::Ptr_suffixContext *ptrSuffix, Type &outPointeeType, int &outPointerDepth) const;
                bool isZeroLiteralExpr(ifccParser::ExprContext *ctx) const;
                int cptTempVariables = 0;
                int scopeCounter = 0;
                bool breakTriggered = false;
                Type currentDeclType = IntType;
                Type currentDeclPointeeType = IntType;
                int currentDeclPointerDepth = 0;
                Type currentFunctionReturnType = IntType;
                Type currentFunctionReturnPointeeType = IntType;
                int currentFunctionReturnPointerDepth = 0;
                map<string, Type> functionReturnType = {{"putchar", IntType}, {"getchar", IntType}};
                map<string, Type> functionReturnPointeeType = {{"putchar", IntType}, {"getchar", IntType}};
                map<string, int> functionReturnPointerDepth = {{"putchar", 0}, {"getchar", 0}};
                map<string, vector<Type>> functionParamTypes = {{"putchar", {IntType}}, {"getchar", {}}};
                map<string, vector<Type>> functionParamPointeeTypes = {{"putchar", {IntType}}, {"getchar", {}}};
                map<string, vector<int>> functionParamPointerDepths = {{"putchar", {0}}, {"getchar", {}}};
                IR ir;
                map<string, int> arraySizeByScopedName;
                map<string, Type> pointerPointeeTypeByScopedName;
                map<string, int> pointerDepthByScopedName;
                // Scope stack: each entry maps source name -> scoped IR name
                vector<map<string, string>> scopeStack;


};
