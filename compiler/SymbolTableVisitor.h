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

        virtual antlrcpp::Any visitDeclVoid(ifccParser::DeclVoidContext *ctx) override ;
        virtual antlrcpp::Any visitDeclConst(ifccParser::DeclConstContext *ctx) override ;
        virtual antlrcpp::Any visitDeclVar(ifccParser::DeclVarContext *ctx) override ;

        virtual antlrcpp::Any visitAffectConst(ifccParser::AffectConstContext *ctx) override ;
        virtual antlrcpp::Any visitAffectVar(ifccParser::AffectVarContext *ctx) override ;  

        virtual antlrcpp::Any visitReturnConst(ifccParser::ReturnConstContext *ctx) override ;
        virtual antlrcpp::Any visitReturnVar(ifccParser::ReturnVarContext *ctx) override ;

        map<string, int> getSymbolTable() const { return symbolTable; }
    private:
        map<string, int> symbolTable;
        set<string> usedVars;
        int nextIndex = 0;
};