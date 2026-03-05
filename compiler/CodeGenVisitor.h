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

        virtual antlrcpp::Any visitDeclList(ifccParser::DeclListContext *ctx) override;
        virtual antlrcpp::Any visitDeclItemVoid(ifccParser::DeclItemVoidContext *ctx) override;
        virtual antlrcpp::Any visitDeclItemConst(ifccParser::DeclItemConstContext *ctx) override;
        virtual antlrcpp::Any visitDeclItemVar(ifccParser::DeclItemVarContext *ctx) override;

        virtual antlrcpp::Any visitAffectConst(ifccParser::AffectConstContext *ctx) override;
        virtual antlrcpp::Any visitAffectVar(ifccParser::AffectVarContext *ctx) override;

        virtual antlrcpp::Any visitReturnConst(ifccParser::ReturnConstContext *ctx) override;
        virtual antlrcpp::Any visitReturnVar(ifccParser::ReturnVarContext *ctx) override;
};
