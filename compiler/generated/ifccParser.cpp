
// Generated from ifcc.g4 by ANTLR 4.9


#include "ifccVisitor.h"

#include "ifccParser.h"


using namespace antlrcpp;
using namespace antlr4;

ifccParser::ifccParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ifccParser::~ifccParser() {
  delete _interpreter;
}

std::string ifccParser::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ifccParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- AxiomContext ------------------------------------------------------------------

ifccParser::AxiomContext::AxiomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::ProgContext* ifccParser::AxiomContext::prog() {
  return getRuleContext<ifccParser::ProgContext>(0);
}

tree::TerminalNode* ifccParser::AxiomContext::EOF() {
  return getToken(ifccParser::EOF, 0);
}


size_t ifccParser::AxiomContext::getRuleIndex() const {
  return ifccParser::RuleAxiom;
}


antlrcpp::Any ifccParser::AxiomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAxiom(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AxiomContext* ifccParser::axiom() {
  AxiomContext *_localctx = _tracker.createInstance<AxiomContext>(_ctx, getState());
  enterRule(_localctx, 0, ifccParser::RuleAxiom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(12);
    prog();
    setState(13);
    match(ifccParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgContext ------------------------------------------------------------------

ifccParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::StatementContext *> ifccParser::ProgContext::statement() {
  return getRuleContexts<ifccParser::StatementContext>();
}

ifccParser::StatementContext* ifccParser::ProgContext::statement(size_t i) {
  return getRuleContext<ifccParser::StatementContext>(i);
}


size_t ifccParser::ProgContext::getRuleIndex() const {
  return ifccParser::RuleProg;
}


antlrcpp::Any ifccParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ProgContext* ifccParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 2, ifccParser::RuleProg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(15);
    match(ifccParser::T__0);
    setState(16);
    match(ifccParser::T__1);
    setState(17);
    match(ifccParser::T__2);
    setState(18);
    match(ifccParser::T__3);
    setState(19);
    match(ifccParser::T__4);
    setState(21); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(20);
      statement();
      setState(23); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ifccParser::T__0)
      | (1ULL << ifccParser::RETURN)
      | (1ULL << ifccParser::VARNAME))) != 0));
    setState(25);
    match(ifccParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

ifccParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::Return_stmtContext* ifccParser::StatementContext::return_stmt() {
  return getRuleContext<ifccParser::Return_stmtContext>(0);
}

ifccParser::Init_stmtContext* ifccParser::StatementContext::init_stmt() {
  return getRuleContext<ifccParser::Init_stmtContext>(0);
}

ifccParser::Assign_stmtContext* ifccParser::StatementContext::assign_stmt() {
  return getRuleContext<ifccParser::Assign_stmtContext>(0);
}


size_t ifccParser::StatementContext::getRuleIndex() const {
  return ifccParser::RuleStatement;
}


antlrcpp::Any ifccParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::StatementContext* ifccParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 4, ifccParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(30);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::RETURN: {
        enterOuterAlt(_localctx, 1);
        setState(27);
        return_stmt();
        break;
      }

      case ifccParser::T__0: {
        enterOuterAlt(_localctx, 2);
        setState(28);
        init_stmt();
        break;
      }

      case ifccParser::VARNAME: {
        enterOuterAlt(_localctx, 3);
        setState(29);
        assign_stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

ifccParser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::Return_stmtContext::getRuleIndex() const {
  return ifccParser::RuleReturn_stmt;
}

void ifccParser::Return_stmtContext::copyFrom(Return_stmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Return_const_stmtContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::Return_const_stmtContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

tree::TerminalNode* ifccParser::Return_const_stmtContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::Return_const_stmtContext::Return_const_stmtContext(Return_stmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ifccParser::Return_const_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturn_const_stmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Return_var_stmtContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::Return_var_stmtContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

tree::TerminalNode* ifccParser::Return_var_stmtContext::VARNAME() {
  return getToken(ifccParser::VARNAME, 0);
}

ifccParser::Return_var_stmtContext::Return_var_stmtContext(Return_stmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ifccParser::Return_var_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturn_var_stmt(this);
  else
    return visitor->visitChildren(this);
}
ifccParser::Return_stmtContext* ifccParser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 6, ifccParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(38);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<Return_stmtContext *>(_tracker.createInstance<ifccParser::Return_const_stmtContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(32);
      match(ifccParser::RETURN);
      setState(33);
      match(ifccParser::CONST);
      setState(34);
      match(ifccParser::T__6);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<Return_stmtContext *>(_tracker.createInstance<ifccParser::Return_var_stmtContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(35);
      match(ifccParser::RETURN);
      setState(36);
      match(ifccParser::VARNAME);
      setState(37);
      match(ifccParser::T__6);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Init_stmtContext ------------------------------------------------------------------

ifccParser::Init_stmtContext::Init_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::Init_stmtContext::getRuleIndex() const {
  return ifccParser::RuleInit_stmt;
}

void ifccParser::Init_stmtContext::copyFrom(Init_stmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Init_stmt_constContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::Init_stmt_constContext::VARNAME() {
  return getToken(ifccParser::VARNAME, 0);
}

tree::TerminalNode* ifccParser::Init_stmt_constContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::Init_stmt_constContext::Init_stmt_constContext(Init_stmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ifccParser::Init_stmt_constContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitInit_stmt_const(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Init_stmt_no_constContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::Init_stmt_no_constContext::VARNAME() {
  return getToken(ifccParser::VARNAME, 0);
}

ifccParser::Init_stmt_no_constContext::Init_stmt_no_constContext(Init_stmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ifccParser::Init_stmt_no_constContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitInit_stmt_no_const(this);
  else
    return visitor->visitChildren(this);
}
ifccParser::Init_stmtContext* ifccParser::init_stmt() {
  Init_stmtContext *_localctx = _tracker.createInstance<Init_stmtContext>(_ctx, getState());
  enterRule(_localctx, 8, ifccParser::RuleInit_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(48);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<Init_stmtContext *>(_tracker.createInstance<ifccParser::Init_stmt_constContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(40);
      match(ifccParser::T__0);
      setState(41);
      match(ifccParser::VARNAME);
      setState(42);
      match(ifccParser::T__7);
      setState(43);
      match(ifccParser::CONST);
      setState(44);
      match(ifccParser::T__6);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<Init_stmtContext *>(_tracker.createInstance<ifccParser::Init_stmt_no_constContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(45);
      match(ifccParser::T__0);
      setState(46);
      match(ifccParser::VARNAME);
      setState(47);
      match(ifccParser::T__6);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Assign_stmtContext ------------------------------------------------------------------

ifccParser::Assign_stmtContext::Assign_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::Assign_stmtContext::getRuleIndex() const {
  return ifccParser::RuleAssign_stmt;
}

void ifccParser::Assign_stmtContext::copyFrom(Assign_stmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Assign_stmt_constContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::Assign_stmt_constContext::VARNAME() {
  return getToken(ifccParser::VARNAME, 0);
}

tree::TerminalNode* ifccParser::Assign_stmt_constContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::Assign_stmt_constContext::Assign_stmt_constContext(Assign_stmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ifccParser::Assign_stmt_constContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAssign_stmt_const(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Assign_stmt_varContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ifccParser::Assign_stmt_varContext::VARNAME() {
  return getTokens(ifccParser::VARNAME);
}

tree::TerminalNode* ifccParser::Assign_stmt_varContext::VARNAME(size_t i) {
  return getToken(ifccParser::VARNAME, i);
}

ifccParser::Assign_stmt_varContext::Assign_stmt_varContext(Assign_stmtContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ifccParser::Assign_stmt_varContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAssign_stmt_var(this);
  else
    return visitor->visitChildren(this);
}
ifccParser::Assign_stmtContext* ifccParser::assign_stmt() {
  Assign_stmtContext *_localctx = _tracker.createInstance<Assign_stmtContext>(_ctx, getState());
  enterRule(_localctx, 10, ifccParser::RuleAssign_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(58);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<Assign_stmtContext *>(_tracker.createInstance<ifccParser::Assign_stmt_varContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(50);
      match(ifccParser::VARNAME);
      setState(51);
      match(ifccParser::T__7);
      setState(52);
      match(ifccParser::VARNAME);
      setState(53);
      match(ifccParser::T__6);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<Assign_stmtContext *>(_tracker.createInstance<ifccParser::Assign_stmt_constContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(54);
      match(ifccParser::VARNAME);
      setState(55);
      match(ifccParser::T__7);
      setState(56);
      match(ifccParser::CONST);
      setState(57);
      match(ifccParser::T__6);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> ifccParser::_decisionToDFA;
atn::PredictionContextCache ifccParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ifccParser::_atn;
std::vector<uint16_t> ifccParser::_serializedATN;

std::vector<std::string> ifccParser::_ruleNames = {
  "axiom", "prog", "statement", "return_stmt", "init_stmt", "assign_stmt"
};

std::vector<std::string> ifccParser::_literalNames = {
  "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "';'", "'='", "'return'"
};

std::vector<std::string> ifccParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "RETURN", "CONST", "COMMENT", "DIRECTIVE", 
  "WS", "VARNAME"
};

dfa::Vocabulary ifccParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ifccParser::_tokenNames;

ifccParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x10, 0x3f, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x6, 0x3, 0x18, 0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x19, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x21, 0xa, 0x4, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 
    0x29, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x33, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 
    0x3d, 0xa, 0x7, 0x3, 0x7, 0x2, 0x2, 0x8, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 
    0x2, 0x2, 0x2, 0x3e, 0x2, 0xe, 0x3, 0x2, 0x2, 0x2, 0x4, 0x11, 0x3, 0x2, 
    0x2, 0x2, 0x6, 0x20, 0x3, 0x2, 0x2, 0x2, 0x8, 0x28, 0x3, 0x2, 0x2, 0x2, 
    0xa, 0x32, 0x3, 0x2, 0x2, 0x2, 0xc, 0x3c, 0x3, 0x2, 0x2, 0x2, 0xe, 0xf, 
    0x5, 0x4, 0x3, 0x2, 0xf, 0x10, 0x7, 0x2, 0x2, 0x3, 0x10, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x11, 0x12, 0x7, 0x3, 0x2, 0x2, 0x12, 0x13, 0x7, 0x4, 0x2, 
    0x2, 0x13, 0x14, 0x7, 0x5, 0x2, 0x2, 0x14, 0x15, 0x7, 0x6, 0x2, 0x2, 
    0x15, 0x17, 0x7, 0x7, 0x2, 0x2, 0x16, 0x18, 0x5, 0x6, 0x4, 0x2, 0x17, 
    0x16, 0x3, 0x2, 0x2, 0x2, 0x18, 0x19, 0x3, 0x2, 0x2, 0x2, 0x19, 0x17, 
    0x3, 0x2, 0x2, 0x2, 0x19, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x1b, 0x3, 
    0x2, 0x2, 0x2, 0x1b, 0x1c, 0x7, 0x8, 0x2, 0x2, 0x1c, 0x5, 0x3, 0x2, 
    0x2, 0x2, 0x1d, 0x21, 0x5, 0x8, 0x5, 0x2, 0x1e, 0x21, 0x5, 0xa, 0x6, 
    0x2, 0x1f, 0x21, 0x5, 0xc, 0x7, 0x2, 0x20, 0x1d, 0x3, 0x2, 0x2, 0x2, 
    0x20, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x20, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x21, 
    0x7, 0x3, 0x2, 0x2, 0x2, 0x22, 0x23, 0x7, 0xb, 0x2, 0x2, 0x23, 0x24, 
    0x7, 0xc, 0x2, 0x2, 0x24, 0x29, 0x7, 0x9, 0x2, 0x2, 0x25, 0x26, 0x7, 
    0xb, 0x2, 0x2, 0x26, 0x27, 0x7, 0x10, 0x2, 0x2, 0x27, 0x29, 0x7, 0x9, 
    0x2, 0x2, 0x28, 0x22, 0x3, 0x2, 0x2, 0x2, 0x28, 0x25, 0x3, 0x2, 0x2, 
    0x2, 0x29, 0x9, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x7, 0x3, 0x2, 0x2, 
    0x2b, 0x2c, 0x7, 0x10, 0x2, 0x2, 0x2c, 0x2d, 0x7, 0xa, 0x2, 0x2, 0x2d, 
    0x2e, 0x7, 0xc, 0x2, 0x2, 0x2e, 0x33, 0x7, 0x9, 0x2, 0x2, 0x2f, 0x30, 
    0x7, 0x3, 0x2, 0x2, 0x30, 0x31, 0x7, 0x10, 0x2, 0x2, 0x31, 0x33, 0x7, 
    0x9, 0x2, 0x2, 0x32, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x32, 0x2f, 0x3, 0x2, 
    0x2, 0x2, 0x33, 0xb, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x7, 0x10, 0x2, 
    0x2, 0x35, 0x36, 0x7, 0xa, 0x2, 0x2, 0x36, 0x37, 0x7, 0x10, 0x2, 0x2, 
    0x37, 0x3d, 0x7, 0x9, 0x2, 0x2, 0x38, 0x39, 0x7, 0x10, 0x2, 0x2, 0x39, 
    0x3a, 0x7, 0xa, 0x2, 0x2, 0x3a, 0x3b, 0x7, 0xc, 0x2, 0x2, 0x3b, 0x3d, 
    0x7, 0x9, 0x2, 0x2, 0x3c, 0x34, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x38, 0x3, 
    0x2, 0x2, 0x2, 0x3d, 0xd, 0x3, 0x2, 0x2, 0x2, 0x7, 0x19, 0x20, 0x28, 
    0x32, 0x3c, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ifccParser::Initializer ifccParser::_init;
