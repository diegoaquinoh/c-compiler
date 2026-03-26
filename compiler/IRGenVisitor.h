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
        int nextFreeSymbolIndex;
        IRGenVisitor() {}

        void add_to_symbol_table(string name, Type t);

        virtual string createVariableTmp(Type t = IntType);

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

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
        virtual antlrcpp::Any visitParens(ifccParser::ParensContext *ctx) override;

        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;

        virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override;
        virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;

        virtual antlrcpp::Any visitBitwiseand(ifccParser::BitwiseandContext *ctx) override;
        virtual antlrcpp::Any visitBitwisexor(ifccParser::BitwisexorContext *ctx) override;
        virtual antlrcpp::Any visitBitwiseor(ifccParser::BitwiseorContext *ctx) override;

        virtual antlrcpp::Any visitRelational(ifccParser::RelationalContext *ctx) override;
        virtual antlrcpp::Any visitEquality(ifccParser::EqualityContext *ctx) override;

        virtual antlrcpp::Any visitLogicalnot(ifccParser::LogicalnotContext *ctx) override;

        virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

        IR& getIR() { return this->ir; }
        
        private:
                Type inferExprType(ifccParser::ExprContext *ctx);
                string activeReg(Type t) const;
                void emitConvert(Type src, Type dst, const string &srcName, const string &dstName);
                void ensureValueInReg(Type currentType, Type targetType);
                int cptTempVariables = 0;
                bool breakTriggered = false;
                Type currentDeclType = IntType;
                IR ir;


};
