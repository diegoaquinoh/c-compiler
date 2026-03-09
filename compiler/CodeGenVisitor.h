#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <map>
#include <string>
using namespace std;


class CodeGenVisitor : public ifccBaseVisitor {
	public:
        CodeGenVisitor(const map<string, int> &symTable, bool hasRet, bool hasFunc) : symbolTable(symTable), hasReturn(hasRet), hasFuncCall(hasFunc) {}

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

        // Statement visitors
        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override;

        virtual antlrcpp::Any visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) override;
        
        virtual antlrcpp::Any visitExpr_stmt(ifccParser::Expr_stmtContext *ctx) override;

        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        // Expression visitors
        virtual antlrcpp::Any visitNegative(ifccParser::NegativeContext *ctx) override;

        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;

        virtual antlrcpp::Any visitBitwiseand(ifccParser::BitwiseandContext *ctx) override;
        virtual antlrcpp::Any visitBitwisexor(ifccParser::BitwisexorContext *ctx) override;
        virtual antlrcpp::Any visitBitwiseor(ifccParser::BitwiseorContext *ctx) override;

        // Return statement visitor
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

        private:
            map<string, int> symbolTable;
            bool hasReturn;
            bool hasFuncCall;
};
