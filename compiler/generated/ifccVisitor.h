
// Generated from ifcc.g4 by ANTLR 4.13.2

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
    virtual std::any visitAxiom(ifccParser::AxiomContext *context) = 0;

    virtual std::any visitProg(ifccParser::ProgContext *context) = 0;

    virtual std::any visitDeclList(ifccParser::DeclListContext *context) = 0;

    virtual std::any visitAffectConst(ifccParser::AffectConstContext *context) = 0;

    virtual std::any visitAffectVar(ifccParser::AffectVarContext *context) = 0;

    virtual std::any visitDeclItemConst(ifccParser::DeclItemConstContext *context) = 0;

    virtual std::any visitDeclItemVar(ifccParser::DeclItemVarContext *context) = 0;

    virtual std::any visitDeclItemVoid(ifccParser::DeclItemVoidContext *context) = 0;

    virtual std::any visitReturnConst(ifccParser::ReturnConstContext *context) = 0;

    virtual std::any visitReturnVar(ifccParser::ReturnVarContext *context) = 0;


};

