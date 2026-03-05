
// Generated from ifcc.g4 by ANTLR 4.9

#pragma once


#include "antlr4-runtime.h"
#include "ifccVisitor.h"


/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ifccBaseVisitor : public ifccVisitor {
public:

  virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturn_const_stmt(ifccParser::Return_const_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturn_var_stmt(ifccParser::Return_var_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInit_stmt_const(ifccParser::Init_stmt_constContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInit_stmt_no_const(ifccParser::Init_stmt_no_constContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssign_stmt_var(ifccParser::Assign_stmt_varContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssign_stmt_const(ifccParser::Assign_stmt_constContext *ctx) override {
    return visitChildren(ctx);
  }


};

