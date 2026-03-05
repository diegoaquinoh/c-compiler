#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <map>
#include <string>
using namespace std;


class CodeGenVisitor : public ifccBaseVisitor {
	public:
        map<string, int> symbolTable;
        CodeGenVisitor(const map<string, int> &symTable) : symbolTable(symTable) {}

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override;

        virtual antlrcpp::Any visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) override;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
};
