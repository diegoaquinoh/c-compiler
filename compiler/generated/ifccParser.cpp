
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccVisitor.h"

#include "ifccParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct IfccParserStaticData final {
  IfccParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccParserStaticData(const IfccParserStaticData&) = delete;
  IfccParserStaticData(IfccParserStaticData&&) = delete;
  IfccParserStaticData& operator=(const IfccParserStaticData&) = delete;
  IfccParserStaticData& operator=(IfccParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifccParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccParserStaticData> ifccParserStaticData = nullptr;

void ifccParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifccParserStaticData != nullptr) {
    return;
  }
#else
  assert(ifccParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccParserStaticData>(
    std::vector<std::string>{
      "axiom", "prog", "stmt", "return_stmt"
    },
    std::vector<std::string>{
      "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "','", "';'", "'='", 
      "'return'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "RETURN", "VAR", "CONST", 
      "COMMENT", "DIRECTIVE", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,15,82,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,1,0,1,0,1,0,1,1,1,1,1,1,1,1,
  	1,1,1,1,5,1,18,8,1,10,1,12,1,21,9,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,5,2,30,
  	8,2,10,2,12,2,33,9,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,2,44,8,2,10,
  	2,12,2,47,9,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,2,58,8,2,10,2,12,
  	2,61,9,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,3,2,72,8,2,1,3,1,3,1,3,1,
  	3,1,3,1,3,3,3,80,8,3,1,3,0,0,4,0,2,4,6,0,0,86,0,8,1,0,0,0,2,11,1,0,0,
  	0,4,71,1,0,0,0,6,79,1,0,0,0,8,9,3,2,1,0,9,10,5,0,0,1,10,1,1,0,0,0,11,
  	12,5,1,0,0,12,13,5,2,0,0,13,14,5,3,0,0,14,15,5,4,0,0,15,19,5,5,0,0,16,
  	18,3,4,2,0,17,16,1,0,0,0,18,21,1,0,0,0,19,17,1,0,0,0,19,20,1,0,0,0,20,
  	22,1,0,0,0,21,19,1,0,0,0,22,23,3,6,3,0,23,24,5,6,0,0,24,3,1,0,0,0,25,
  	26,5,1,0,0,26,31,5,11,0,0,27,28,5,7,0,0,28,30,5,11,0,0,29,27,1,0,0,0,
  	30,33,1,0,0,0,31,29,1,0,0,0,31,32,1,0,0,0,32,34,1,0,0,0,33,31,1,0,0,0,
  	34,72,5,8,0,0,35,36,5,1,0,0,36,37,5,11,0,0,37,38,5,9,0,0,38,45,5,12,0,
  	0,39,40,5,7,0,0,40,41,5,11,0,0,41,42,5,9,0,0,42,44,5,12,0,0,43,39,1,0,
  	0,0,44,47,1,0,0,0,45,43,1,0,0,0,45,46,1,0,0,0,46,48,1,0,0,0,47,45,1,0,
  	0,0,48,72,5,8,0,0,49,50,5,1,0,0,50,51,5,11,0,0,51,52,5,9,0,0,52,59,5,
  	11,0,0,53,54,5,7,0,0,54,55,5,11,0,0,55,56,5,9,0,0,56,58,5,11,0,0,57,53,
  	1,0,0,0,58,61,1,0,0,0,59,57,1,0,0,0,59,60,1,0,0,0,60,62,1,0,0,0,61,59,
  	1,0,0,0,62,72,5,8,0,0,63,64,5,11,0,0,64,65,5,9,0,0,65,66,5,12,0,0,66,
  	72,5,8,0,0,67,68,5,11,0,0,68,69,5,9,0,0,69,70,5,11,0,0,70,72,5,8,0,0,
  	71,25,1,0,0,0,71,35,1,0,0,0,71,49,1,0,0,0,71,63,1,0,0,0,71,67,1,0,0,0,
  	72,5,1,0,0,0,73,74,5,10,0,0,74,75,5,12,0,0,75,80,5,8,0,0,76,77,5,10,0,
  	0,77,78,5,11,0,0,78,80,5,8,0,0,79,73,1,0,0,0,79,76,1,0,0,0,80,7,1,0,0,
  	0,6,19,31,45,59,71,79
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifccParserStaticData = std::move(staticData);
}

}

ifccParser::ifccParser(TokenStream *input) : ifccParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ifccParser::ifccParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ifccParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ifccParserStaticData->atn, ifccParserStaticData->decisionToDFA, ifccParserStaticData->sharedContextCache, options);
}

ifccParser::~ifccParser() {
  delete _interpreter;
}

const atn::ATN& ifccParser::getATN() const {
  return *ifccParserStaticData->atn;
}

std::string ifccParser::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccParser::getRuleNames() const {
  return ifccParserStaticData->ruleNames;
}

const dfa::Vocabulary& ifccParser::getVocabulary() const {
  return ifccParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccParser::getSerializedATN() const {
  return ifccParserStaticData->serializedATN;
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


std::any ifccParser::AxiomContext::accept(tree::ParseTreeVisitor *visitor) {
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
    setState(8);
    prog();
    setState(9);
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

ifccParser::Return_stmtContext* ifccParser::ProgContext::return_stmt() {
  return getRuleContext<ifccParser::Return_stmtContext>(0);
}

std::vector<ifccParser::StmtContext *> ifccParser::ProgContext::stmt() {
  return getRuleContexts<ifccParser::StmtContext>();
}

ifccParser::StmtContext* ifccParser::ProgContext::stmt(size_t i) {
  return getRuleContext<ifccParser::StmtContext>(i);
}


size_t ifccParser::ProgContext::getRuleIndex() const {
  return ifccParser::RuleProg;
}


std::any ifccParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
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
    setState(11);
    match(ifccParser::T__0);
    setState(12);
    match(ifccParser::T__1);
    setState(13);
    match(ifccParser::T__2);
    setState(14);
    match(ifccParser::T__3);
    setState(15);
    match(ifccParser::T__4);
    setState(19);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::T__0

    || _la == ifccParser::VAR) {
      setState(16);
      stmt();
      setState(21);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(22);
    return_stmt();
    setState(23);
    match(ifccParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

ifccParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::StmtContext::getRuleIndex() const {
  return ifccParser::RuleStmt;
}

void ifccParser::StmtContext::copyFrom(StmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DeclConstContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ifccParser::DeclConstContext::VAR() {
  return getTokens(ifccParser::VAR);
}

tree::TerminalNode* ifccParser::DeclConstContext::VAR(size_t i) {
  return getToken(ifccParser::VAR, i);
}

std::vector<tree::TerminalNode *> ifccParser::DeclConstContext::CONST() {
  return getTokens(ifccParser::CONST);
}

tree::TerminalNode* ifccParser::DeclConstContext::CONST(size_t i) {
  return getToken(ifccParser::CONST, i);
}

ifccParser::DeclConstContext::DeclConstContext(StmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclConstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclConst(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DeclVarContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ifccParser::DeclVarContext::VAR() {
  return getTokens(ifccParser::VAR);
}

tree::TerminalNode* ifccParser::DeclVarContext::VAR(size_t i) {
  return getToken(ifccParser::VAR, i);
}

ifccParser::DeclVarContext::DeclVarContext(StmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AffectVarContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ifccParser::AffectVarContext::VAR() {
  return getTokens(ifccParser::VAR);
}

tree::TerminalNode* ifccParser::AffectVarContext::VAR(size_t i) {
  return getToken(ifccParser::VAR, i);
}

ifccParser::AffectVarContext::AffectVarContext(StmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::AffectVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAffectVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DeclVoidContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ifccParser::DeclVoidContext::VAR() {
  return getTokens(ifccParser::VAR);
}

tree::TerminalNode* ifccParser::DeclVoidContext::VAR(size_t i) {
  return getToken(ifccParser::VAR, i);
}

ifccParser::DeclVoidContext::DeclVoidContext(StmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclVoidContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclVoid(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AffectConstContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::AffectConstContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

tree::TerminalNode* ifccParser::AffectConstContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::AffectConstContext::AffectConstContext(StmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::AffectConstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAffectConst(this);
  else
    return visitor->visitChildren(this);
}
ifccParser::StmtContext* ifccParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 4, ifccParser::RuleStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(71);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ifccParser::DeclVoidContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(25);
      match(ifccParser::T__0);
      setState(26);
      match(ifccParser::VAR);
      setState(31);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ifccParser::T__6) {
        setState(27);
        match(ifccParser::T__6);
        setState(28);
        match(ifccParser::VAR);
        setState(33);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(34);
      match(ifccParser::T__7);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ifccParser::DeclConstContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(35);
      match(ifccParser::T__0);
      setState(36);
      match(ifccParser::VAR);
      setState(37);
      match(ifccParser::T__8);
      setState(38);
      match(ifccParser::CONST);
      setState(45);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ifccParser::T__6) {
        setState(39);
        match(ifccParser::T__6);
        setState(40);
        match(ifccParser::VAR);
        setState(41);
        match(ifccParser::T__8);
        setState(42);
        match(ifccParser::CONST);
        setState(47);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(48);
      match(ifccParser::T__7);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ifccParser::DeclVarContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(49);
      match(ifccParser::T__0);
      setState(50);
      match(ifccParser::VAR);
      setState(51);
      match(ifccParser::T__8);
      setState(52);
      match(ifccParser::VAR);
      setState(59);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ifccParser::T__6) {
        setState(53);
        match(ifccParser::T__6);
        setState(54);
        match(ifccParser::VAR);
        setState(55);
        match(ifccParser::T__8);
        setState(56);
        match(ifccParser::VAR);
        setState(61);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(62);
      match(ifccParser::T__7);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<ifccParser::AffectConstContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(63);
      match(ifccParser::VAR);
      setState(64);
      match(ifccParser::T__8);
      setState(65);
      match(ifccParser::CONST);
      setState(66);
      match(ifccParser::T__7);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ifccParser::AffectVarContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(67);
      match(ifccParser::VAR);
      setState(68);
      match(ifccParser::T__8);
      setState(69);
      match(ifccParser::VAR);
      setState(70);
      match(ifccParser::T__7);
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

//----------------- ReturnVarContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::ReturnVarContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

tree::TerminalNode* ifccParser::ReturnVarContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

ifccParser::ReturnVarContext::ReturnVarContext(Return_stmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ReturnVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturnVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnConstContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::ReturnConstContext::RETURN() {
  return getToken(ifccParser::RETURN, 0);
}

tree::TerminalNode* ifccParser::ReturnConstContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::ReturnConstContext::ReturnConstContext(Return_stmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ReturnConstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturnConst(this);
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
    setState(79);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ifccParser::ReturnConstContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(73);
      match(ifccParser::RETURN);
      setState(74);
      match(ifccParser::CONST);
      setState(75);
      match(ifccParser::T__7);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ifccParser::ReturnVarContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(76);
      match(ifccParser::RETURN);
      setState(77);
      match(ifccParser::VAR);
      setState(78);
      match(ifccParser::T__7);
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

void ifccParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifccParserInitialize();
#else
  ::antlr4::internal::call_once(ifccParserOnceFlag, ifccParserInitialize);
#endif
}
