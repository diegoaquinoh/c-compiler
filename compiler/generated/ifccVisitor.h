
// Generated from ifcc.g4 by ANTLR 4.9

#pragma once


#include "antlr4-runtime.h"
#include "ifccParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ifccParser.
 */
class  ifccVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ifccParser.
   */
    virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *context) = 0;

    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *context) = 0;

    virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitReturn_const_stmt(ifccParser::Return_const_stmtContext *context) = 0;

    virtual antlrcpp::Any visitReturn_var_stmt(ifccParser::Return_var_stmtContext *context) = 0;

    virtual antlrcpp::Any visitInit_stmt_const(ifccParser::Init_stmt_constContext *context) = 0;

    virtual antlrcpp::Any visitInit_stmt_no_const(ifccParser::Init_stmt_no_constContext *context) = 0;

    virtual antlrcpp::Any visitAssign_stmt_var(ifccParser::Assign_stmt_varContext *context) = 0;

    virtual antlrcpp::Any visitAssign_stmt_const(ifccParser::Assign_stmt_constContext *context) = 0;


};

