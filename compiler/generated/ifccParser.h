
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ifccParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, RETURN = 9, VAR = 10, CONST = 11, COMMENT = 12, DIRECTIVE = 13, 
    WS = 14
  };

  enum {
    RuleAxiom = 0, RuleProg = 1, RuleStmt = 2, RuleReturn_stmt = 3
  };

  explicit ifccParser(antlr4::TokenStream *input);

  ifccParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~ifccParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class AxiomContext;
  class ProgContext;
  class StmtContext;
  class Return_stmtContext; 

  class  AxiomContext : public antlr4::ParserRuleContext {
  public:
    AxiomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProgContext *prog();
    antlr4::tree::TerminalNode *EOF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AxiomContext* axiom();

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Return_stmtContext *return_stmt();
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    StmtContext() = default;
    void copyFrom(StmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DeclConstContext : public StmtContext {
  public:
    DeclConstContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *CONST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DeclVarContext : public StmtContext {
  public:
    DeclVarContext(StmtContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> VAR();
    antlr4::tree::TerminalNode* VAR(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AffectVarContext : public StmtContext {
  public:
    AffectVarContext(StmtContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> VAR();
    antlr4::tree::TerminalNode* VAR(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DeclVoidContext : public StmtContext {
  public:
    DeclVoidContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *VAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AffectConstContext : public StmtContext {
  public:
    AffectConstContext(StmtContext *ctx);

    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *CONST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  StmtContext* stmt();

  class  Return_stmtContext : public antlr4::ParserRuleContext {
  public:
    Return_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Return_stmtContext() = default;
    void copyFrom(Return_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnVarContext : public Return_stmtContext {
  public:
    ReturnVarContext(Return_stmtContext *ctx);

    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *VAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnConstContext : public Return_stmtContext {
  public:
    ReturnConstContext(Return_stmtContext *ctx);

    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *CONST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Return_stmtContext* return_stmt();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

