
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct IfccLexerStaticData final {
  IfccLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccLexerStaticData(const IfccLexerStaticData&) = delete;
  IfccLexerStaticData(IfccLexerStaticData&&) = delete;
  IfccLexerStaticData& operator=(const IfccLexerStaticData&) = delete;
  IfccLexerStaticData& operator=(IfccLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifcclexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccLexerStaticData> ifcclexerLexerStaticData = nullptr;

void ifcclexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifcclexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(ifcclexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "RETURN", "VAR", "CONST", "COMMENT", "DIRECTIVE", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,15,102,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,
  	1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,5,10,
  	64,8,10,10,10,12,10,67,9,10,1,11,4,11,70,8,11,11,11,12,11,71,1,12,1,12,
  	1,12,1,12,5,12,78,8,12,10,12,12,12,81,9,12,1,12,1,12,1,12,1,12,1,12,1,
  	13,1,13,5,13,90,8,13,10,13,12,13,93,9,13,1,13,1,13,1,13,1,13,1,14,1,14,
  	1,14,1,14,2,79,91,0,15,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,
  	21,11,23,12,25,13,27,14,29,15,1,0,4,3,0,65,90,95,95,97,122,4,0,48,57,
  	65,90,95,95,97,122,1,0,48,57,3,0,9,10,13,13,32,32,105,0,1,1,0,0,0,0,3,
  	1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,
  	0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,
  	1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,1,31,1,0,0,0,3,35,1,0,0,0,5,40,1,0,
  	0,0,7,42,1,0,0,0,9,44,1,0,0,0,11,46,1,0,0,0,13,48,1,0,0,0,15,50,1,0,0,
  	0,17,52,1,0,0,0,19,54,1,0,0,0,21,61,1,0,0,0,23,69,1,0,0,0,25,73,1,0,0,
  	0,27,87,1,0,0,0,29,98,1,0,0,0,31,32,5,105,0,0,32,33,5,110,0,0,33,34,5,
  	116,0,0,34,2,1,0,0,0,35,36,5,109,0,0,36,37,5,97,0,0,37,38,5,105,0,0,38,
  	39,5,110,0,0,39,4,1,0,0,0,40,41,5,40,0,0,41,6,1,0,0,0,42,43,5,41,0,0,
  	43,8,1,0,0,0,44,45,5,123,0,0,45,10,1,0,0,0,46,47,5,125,0,0,47,12,1,0,
  	0,0,48,49,5,44,0,0,49,14,1,0,0,0,50,51,5,59,0,0,51,16,1,0,0,0,52,53,5,
  	61,0,0,53,18,1,0,0,0,54,55,5,114,0,0,55,56,5,101,0,0,56,57,5,116,0,0,
  	57,58,5,117,0,0,58,59,5,114,0,0,59,60,5,110,0,0,60,20,1,0,0,0,61,65,7,
  	0,0,0,62,64,7,1,0,0,63,62,1,0,0,0,64,67,1,0,0,0,65,63,1,0,0,0,65,66,1,
  	0,0,0,66,22,1,0,0,0,67,65,1,0,0,0,68,70,7,2,0,0,69,68,1,0,0,0,70,71,1,
  	0,0,0,71,69,1,0,0,0,71,72,1,0,0,0,72,24,1,0,0,0,73,74,5,47,0,0,74,75,
  	5,42,0,0,75,79,1,0,0,0,76,78,9,0,0,0,77,76,1,0,0,0,78,81,1,0,0,0,79,80,
  	1,0,0,0,79,77,1,0,0,0,80,82,1,0,0,0,81,79,1,0,0,0,82,83,5,42,0,0,83,84,
  	5,47,0,0,84,85,1,0,0,0,85,86,6,12,0,0,86,26,1,0,0,0,87,91,5,35,0,0,88,
  	90,9,0,0,0,89,88,1,0,0,0,90,93,1,0,0,0,91,92,1,0,0,0,91,89,1,0,0,0,92,
  	94,1,0,0,0,93,91,1,0,0,0,94,95,5,10,0,0,95,96,1,0,0,0,96,97,6,13,0,0,
  	97,28,1,0,0,0,98,99,7,3,0,0,99,100,1,0,0,0,100,101,6,14,1,0,101,30,1,
  	0,0,0,5,0,65,71,79,91,2,6,0,0,0,1,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifcclexerLexerStaticData = std::move(staticData);
}

}

ifccLexer::ifccLexer(CharStream *input) : Lexer(input) {
  ifccLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *ifcclexerLexerStaticData->atn, ifcclexerLexerStaticData->decisionToDFA, ifcclexerLexerStaticData->sharedContextCache);
}

ifccLexer::~ifccLexer() {
  delete _interpreter;
}

std::string ifccLexer::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccLexer::getRuleNames() const {
  return ifcclexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& ifccLexer::getChannelNames() const {
  return ifcclexerLexerStaticData->channelNames;
}

const std::vector<std::string>& ifccLexer::getModeNames() const {
  return ifcclexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& ifccLexer::getVocabulary() const {
  return ifcclexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccLexer::getSerializedATN() const {
  return ifcclexerLexerStaticData->serializedATN;
}

const atn::ATN& ifccLexer::getATN() const {
  return *ifcclexerLexerStaticData->atn;
}




void ifccLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifcclexerLexerInitialize();
#else
  ::antlr4::internal::call_once(ifcclexerLexerOnceFlag, ifcclexerLexerInitialize);
#endif
}
