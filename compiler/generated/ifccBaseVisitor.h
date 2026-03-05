
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccVisitor.h"


/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ifccBaseVisitor : public ifccVisitor {
public:

  virtual std::any visitAxiom(ifccParser::AxiomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProg(ifccParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclList(ifccParser::DeclListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAffectConst(ifccParser::AffectConstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAffectVar(ifccParser::AffectVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclItemConst(ifccParser::DeclItemConstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclItemVar(ifccParser::DeclItemVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclItemVoid(ifccParser::DeclItemVoidContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnConst(ifccParser::ReturnConstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnVar(ifccParser::ReturnVarContext *ctx) override {
    return visitChildren(ctx);
  }


};

