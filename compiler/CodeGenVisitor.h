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

        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override;

        virtual antlrcpp::Any visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) override;

        virtual antlrcpp::Any visitExpr_stmt(ifccParser::Expr_stmtContext *ctx) override;

        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

        private:
            map<string, int> symbolTable;
            bool hasReturn;
            bool hasFuncCall;
};
