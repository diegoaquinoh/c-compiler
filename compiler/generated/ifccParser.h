
// Generated from ifcc.g4 by ANTLR 4.9

#pragma once


#include "antlr4-runtime.h"




class  ifccParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, RETURN = 9, CONST = 10, COMMENT = 11, DIRECTIVE = 12, WS = 13, 
    VARNAME = 14
  };

  enum {
    RuleAxiom = 0, RuleProg = 1, RuleStatement = 2, RuleReturn_stmt = 3, 
    RuleInit_stmt = 4, RuleAssign_stmt = 5
  };

  explicit ifccParser(antlr4::TokenStream *input);
  ~ifccParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class AxiomContext;
  class ProgContext;
  class StatementContext;
  class Return_stmtContext;
  class Init_stmtContext;
  class Assign_stmtContext; 

  class  AxiomContext : public antlr4::ParserRuleContext {
  public:
    AxiomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProgContext *prog();
    antlr4::tree::TerminalNode *EOF();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AxiomContext* axiom();

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Return_stmtContext *return_stmt();
    Init_stmtContext *init_stmt();
    Assign_stmtContext *assign_stmt();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  Return_stmtContext : public antlr4::ParserRuleContext {
  public:
    Return_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Return_stmtContext() = default;
    void copyFrom(Return_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Return_const_stmtContext : public Return_stmtContext {
  public:
    Return_const_stmtContext(Return_stmtContext *ctx);

    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *CONST();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Return_var_stmtContext : public Return_stmtContext {
  public:
    Return_var_stmtContext(Return_stmtContext *ctx);

    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *VARNAME();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Return_stmtContext* return_stmt();

  class  Init_stmtContext : public antlr4::ParserRuleContext {
  public:
    Init_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Init_stmtContext() = default;
    void copyFrom(Init_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Init_stmt_constContext : public Init_stmtContext {
  public:
    Init_stmt_constContext(Init_stmtContext *ctx);

    antlr4::tree::TerminalNode *VARNAME();
    antlr4::tree::TerminalNode *CONST();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Init_stmt_no_constContext : public Init_stmtContext {
  public:
    Init_stmt_no_constContext(Init_stmtContext *ctx);

    antlr4::tree::TerminalNode *VARNAME();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Init_stmtContext* init_stmt();

  class  Assign_stmtContext : public antlr4::ParserRuleContext {
  public:
    Assign_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Assign_stmtContext() = default;
    void copyFrom(Assign_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Assign_stmt_constContext : public Assign_stmtContext {
  public:
    Assign_stmt_constContext(Assign_stmtContext *ctx);

    antlr4::tree::TerminalNode *VARNAME();
    antlr4::tree::TerminalNode *CONST();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Assign_stmt_varContext : public Assign_stmtContext {
  public:
    Assign_stmt_varContext(Assign_stmtContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> VARNAME();
    antlr4::tree::TerminalNode* VARNAME(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Assign_stmtContext* assign_stmt();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

