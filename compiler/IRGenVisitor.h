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
        map<string, int> symbolTable;
        IRGenVisitor(const map<string, int> &symTable) : symbolTable(symTable) {}

        virtual string createVariableTmp();

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

        // Statement visitors
        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDecl_item(ifccParser::Decl_itemContext *ctx) override;

//        virtual antlrcpp::Any visitCallStmt(ifccParser::CallStmtContext *ctx) override;
//        virtual antlrcpp::Any visitFuncCall(ifccParser::FuncCallContext *ctx) override;

        virtual antlrcpp::Any visitAffect_stmt(ifccParser::Affect_stmtContext *ctx) override;
        
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

//        virtual antlrcpp::Any visitLogicalnot(ifccParser::LogicalnotContext *ctx) override;

        // Return statement visitor
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

        IR getIR() const { return this->ir; }
        
        private:
                int cptTempVariables = 0;
                IR ir;


};
