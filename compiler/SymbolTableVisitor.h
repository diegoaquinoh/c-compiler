#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <map>
#include <set>
#include <string>
using namespace std;

class SymbolTableVisitor: public ifccBaseVisitor {
    public:
        int declareVar(const string &name);
        void useVar(const string &name);

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;

        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override ;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override ;

        virtual antlrcpp::Any visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) override ;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override ;

        map<string, int> getSymbolTable() const { return symbolTable; }
        bool hasError() const { return errorFlag; }
    private:
        map<string, int> symbolTable;
        set<string> usedVars;
        int nextIndex = -4;
        bool errorFlag = false;
};