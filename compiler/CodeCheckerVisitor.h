#pragma once

#include <unordered_map>
#include <string>
#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class  CodeCheckerVisitor : public ifccBaseVisitor {
	public:
        std::unordered_map <std::string, int> symbolTable;
        int nextOffset = -4; 

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_const_stmt(ifccParser::Return_const_stmtContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_var_stmt(ifccParser::Return_var_stmtContext *ctx) override ;
        virtual antlrcpp::Any visitInit_stmt_const(ifccParser::Init_stmt_constContext *ctx) override ;
        virtual antlrcpp::Any visitInit_stmt_no_const(ifccParser::Init_stmt_no_constContext *ctx) override ;
        virtual antlrcpp::Any visitAssign_stmt_var(ifccParser::Assign_stmt_varContext *ctx) override ;
        virtual antlrcpp::Any visitAssign_stmt_const(ifccParser::Assign_stmt_constContext *ctx) override ;

        
};

