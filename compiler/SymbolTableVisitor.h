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
        // Scope management
        void enterScope();
        void exitScope();

        // Variable management
        int declareVar(const std::string &name);
        int lookupVar(const std::string &name);
        void useVar(const std::string &name);

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitFunc_def(ifccParser::Func_defContext *ctx) override;
        virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override ;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override ;

        virtual antlrcpp::Any visitAffectStmt(ifccParser::AffectStmtContext *ctx) override ;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override ;

        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        virtual antlrcpp::Any visitNegative(ifccParser::NegativeContext *ctx) override;
        virtual antlrcpp::Any visitParens(ifccParser::ParensContext *ctx) override;

        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;

        virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;

        virtual antlrcpp::Any visitBitwiseand(ifccParser::BitwiseandContext *ctx) override;
        virtual antlrcpp::Any visitBitwisexor(ifccParser::BitwisexorContext *ctx) override;
        virtual antlrcpp::Any visitBitwiseor(ifccParser::BitwiseorContext *ctx) override;


        map<string, map<string, int>> getAllSymbolTables() const { return allSymbolTables; }
        map<string, int> getFunctionArgCount() const { return functionArgCount; }
        bool hasError() const { return errorFlag; }

    private:
        // Per-function scope stack
        vector<pair<string, int>> varStack;    // (varName, stackOffset)
        vector<int> scopeMarkers;              // marks where each scope begins in varStack

        // Global function registry
        string currentFunction;
        map<string, map<string, int>> allSymbolTables;  // funcName -> (varName -> offset), built at end of each function
        set<string> usedVars;
        int nextIndex = 0;
        bool errorFlag = false;
        set<string> knownFunctions = {"putchar", "getchar"};
        map<string, int> functionArgCount = {{"putchar", 1}, {"getchar", 0}};
};
