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
        virtual antlrcpp::Any visitExpr_stmt(ifccParser::Expr_stmtContext *ctx) override ;
        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override ;
        virtual antlrcpp::Any visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) override ;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override ;

        map<string, int> getSymbolTable() const { return symbolTable; }
        bool getHasReturn() const { return hasReturn; }
        bool getHasFuncCall() const { return hasFuncCall; }
        bool hasError() const { return errorFlag; }
    private:
        map<string, int> symbolTable;
        set<string> usedVars;
        map<string, int> knownFunctions = {{"putchar", 1}, {"getchar", 0}};
        int nextIndex = -4;
        bool errorFlag = false;
        bool hasReturn = false;
        bool hasFuncCall = false;
};