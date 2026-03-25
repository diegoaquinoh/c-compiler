#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "Type.h"
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

        virtual antlrcpp::Any visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) override;
        virtual antlrcpp::Any visitSwitch_clause(ifccParser::Switch_clauseContext *ctx) override;
        virtual antlrcpp::Any visitCase_label(ifccParser::Case_labelContext *ctx) override;
        virtual antlrcpp::Any visitDefault_label(ifccParser::Default_labelContext *ctx) override;
        virtual antlrcpp::Any visitCase_value(ifccParser::Case_valueContext *ctx) override;
        virtual antlrcpp::Any visitBreak_stmt(ifccParser::Break_stmtContext *ctx) override;

        virtual antlrcpp::Any visitAffectStmt(ifccParser::AffectStmtContext *ctx) override ;

        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override ;
        
        virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;

        virtual antlrcpp::Any visitElse_stmt(ifccParser::Else_stmtContext *ctx) override;

        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        virtual antlrcpp::Any visitNegative(ifccParser::NegativeContext *ctx) override;
        virtual antlrcpp::Any visitLogicalnot(ifccParser::LogicalnotContext *ctx) override;
        virtual antlrcpp::Any visitParens(ifccParser::ParensContext *ctx) override;

        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override;
        virtual antlrcpp::Any visitRelational(ifccParser::RelationalContext *ctx) override;
        virtual antlrcpp::Any visitEquality(ifccParser::EqualityContext *ctx) override;

        virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override;
        virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;

        virtual antlrcpp::Any visitBitwiseand(ifccParser::BitwiseandContext *ctx) override;
        virtual antlrcpp::Any visitBitwisexor(ifccParser::BitwisexorContext *ctx) override;
        virtual antlrcpp::Any visitBitwiseor(ifccParser::BitwiseorContext *ctx) override;

        virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

        map<string, int> getSymbolTable() const { return symbolTable; }
        bool hasError() const { return errorFlag; }

    private:
        map<string, int> symbolTable;
        map<string,Type> symbolType;
        set<string> usedVars;
        int nextIndex = -4;
        bool errorFlag = false;
        set<string> knownFunctions = {"putchar", "getchar"};
        map<string, int> functionArgCount = {{"putchar", 1}, {"getchar", 0}};
};