
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
      "axiom", "prog", "stmt", "decl_item", "return_stmt"
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
  	4,1,15,66,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,1,0,1,0,1,0,1,1,1,1,
  	1,1,1,1,1,1,1,1,5,1,20,8,1,10,1,12,1,23,9,1,1,1,1,1,1,1,1,2,1,2,1,2,1,
  	2,5,2,32,8,2,10,2,12,2,35,9,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,
  	3,2,47,8,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,56,8,3,1,4,1,4,1,4,1,4,1,4,
  	1,4,3,4,64,8,4,1,4,0,0,5,0,2,4,6,8,0,0,67,0,10,1,0,0,0,2,13,1,0,0,0,4,
  	46,1,0,0,0,6,55,1,0,0,0,8,63,1,0,0,0,10,11,3,2,1,0,11,12,5,0,0,1,12,1,
  	1,0,0,0,13,14,5,1,0,0,14,15,5,2,0,0,15,16,5,3,0,0,16,17,5,4,0,0,17,21,
  	5,5,0,0,18,20,3,4,2,0,19,18,1,0,0,0,20,23,1,0,0,0,21,19,1,0,0,0,21,22,
  	1,0,0,0,22,24,1,0,0,0,23,21,1,0,0,0,24,25,3,8,4,0,25,26,5,6,0,0,26,3,
  	1,0,0,0,27,28,5,1,0,0,28,33,3,6,3,0,29,30,5,7,0,0,30,32,3,6,3,0,31,29,
  	1,0,0,0,32,35,1,0,0,0,33,31,1,0,0,0,33,34,1,0,0,0,34,36,1,0,0,0,35,33,
  	1,0,0,0,36,37,5,8,0,0,37,47,1,0,0,0,38,39,5,11,0,0,39,40,5,9,0,0,40,41,
  	5,12,0,0,41,47,5,8,0,0,42,43,5,11,0,0,43,44,5,9,0,0,44,45,5,11,0,0,45,
  	47,5,8,0,0,46,27,1,0,0,0,46,38,1,0,0,0,46,42,1,0,0,0,47,5,1,0,0,0,48,
  	49,5,11,0,0,49,50,5,9,0,0,50,56,5,12,0,0,51,52,5,11,0,0,52,53,5,9,0,0,
  	53,56,5,11,0,0,54,56,5,11,0,0,55,48,1,0,0,0,55,51,1,0,0,0,55,54,1,0,0,
  	0,56,7,1,0,0,0,57,58,5,10,0,0,58,59,5,12,0,0,59,64,5,8,0,0,60,61,5,10,
  	0,0,61,62,5,11,0,0,62,64,5,8,0,0,63,57,1,0,0,0,63,60,1,0,0,0,64,9,1,0,
  	0,0,5,21,33,46,55,63
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
    setState(10);
    prog();
    setState(11);
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
    setState(13);
    match(ifccParser::T__0);
    setState(14);
    match(ifccParser::T__1);
    setState(15);
    match(ifccParser::T__2);
    setState(16);
    match(ifccParser::T__3);
    setState(17);
    match(ifccParser::T__4);
    setState(21);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ifccParser::T__0

    || _la == ifccParser::VAR) {
      setState(18);
      stmt();
      setState(23);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(24);
    return_stmt();
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
//----------------- DeclListContext ------------------------------------------------------------------

std::vector<ifccParser::Decl_itemContext *> ifccParser::DeclListContext::decl_item() {
  return getRuleContexts<ifccParser::Decl_itemContext>();
}

ifccParser::Decl_itemContext* ifccParser::DeclListContext::decl_item(size_t i) {
  return getRuleContext<ifccParser::Decl_itemContext>(i);
}

ifccParser::DeclListContext::DeclListContext(StmtContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclList(this);
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
    setState(46);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ifccParser::DeclListContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(27);
      match(ifccParser::T__0);
      setState(28);
      decl_item();
      setState(33);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ifccParser::T__6) {
        setState(29);
        match(ifccParser::T__6);
        setState(30);
        decl_item();
        setState(35);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(36);
      match(ifccParser::T__7);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ifccParser::AffectConstContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(38);
      match(ifccParser::VAR);
      setState(39);
      match(ifccParser::T__8);
      setState(40);
      match(ifccParser::CONST);
      setState(41);
      match(ifccParser::T__7);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ifccParser::AffectVarContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(42);
      match(ifccParser::VAR);
      setState(43);
      match(ifccParser::T__8);
      setState(44);
      match(ifccParser::VAR);
      setState(45);
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

//----------------- Decl_itemContext ------------------------------------------------------------------

ifccParser::Decl_itemContext::Decl_itemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::Decl_itemContext::getRuleIndex() const {
  return ifccParser::RuleDecl_item;
}

void ifccParser::Decl_itemContext::copyFrom(Decl_itemContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DeclItemConstContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::DeclItemConstContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

tree::TerminalNode* ifccParser::DeclItemConstContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

ifccParser::DeclItemConstContext::DeclItemConstContext(Decl_itemContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclItemConstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclItemConst(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DeclItemVarContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ifccParser::DeclItemVarContext::VAR() {
  return getTokens(ifccParser::VAR);
}

tree::TerminalNode* ifccParser::DeclItemVarContext::VAR(size_t i) {
  return getToken(ifccParser::VAR, i);
}

ifccParser::DeclItemVarContext::DeclItemVarContext(Decl_itemContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclItemVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclItemVar(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DeclItemVoidContext ------------------------------------------------------------------

tree::TerminalNode* ifccParser::DeclItemVoidContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

ifccParser::DeclItemVoidContext::DeclItemVoidContext(Decl_itemContext *ctx) { copyFrom(ctx); }


std::any ifccParser::DeclItemVoidContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclItemVoid(this);
  else
    return visitor->visitChildren(this);
}
ifccParser::Decl_itemContext* ifccParser::decl_item() {
  Decl_itemContext *_localctx = _tracker.createInstance<Decl_itemContext>(_ctx, getState());
  enterRule(_localctx, 6, ifccParser::RuleDecl_item);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(55);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ifccParser::DeclItemConstContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(48);
      match(ifccParser::VAR);
      setState(49);
      match(ifccParser::T__8);
      setState(50);
      match(ifccParser::CONST);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ifccParser::DeclItemVarContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(51);
      match(ifccParser::VAR);
      setState(52);
      match(ifccParser::T__8);
      setState(53);
      match(ifccParser::VAR);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ifccParser::DeclItemVoidContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(54);
      match(ifccParser::VAR);
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
  enterRule(_localctx, 8, ifccParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(63);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ifccParser::ReturnConstContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(57);
      match(ifccParser::RETURN);
      setState(58);
      match(ifccParser::CONST);
      setState(59);
      match(ifccParser::T__7);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ifccParser::ReturnVarContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(60);
      match(ifccParser::RETURN);
      setState(61);
      match(ifccParser::VAR);
      setState(62);
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
