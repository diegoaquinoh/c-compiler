
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
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "RETURN", 
      "CONST", "COMMENT", "DIRECTIVE", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "';'", "'return'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "RETURN", "CONST", "COMMENT", "DIRECTIVE", 
      "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,12,85,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,1,0,1,0,1,0,1,0,1,1,1,1,
  	1,1,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,7,1,7,1,
  	7,1,7,1,7,1,8,4,8,53,8,8,11,8,12,8,54,1,9,1,9,1,9,1,9,5,9,61,8,9,10,9,
  	12,9,64,9,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,5,10,73,8,10,10,10,12,10,76,
  	9,10,1,10,1,10,1,10,1,10,1,11,1,11,1,11,1,11,2,62,74,0,12,1,1,3,2,5,3,
  	7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,1,0,2,1,0,48,57,3,0,9,10,
  	13,13,32,32,87,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,
  	0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,
  	0,0,21,1,0,0,0,0,23,1,0,0,0,1,25,1,0,0,0,3,29,1,0,0,0,5,34,1,0,0,0,7,
  	36,1,0,0,0,9,38,1,0,0,0,11,40,1,0,0,0,13,42,1,0,0,0,15,44,1,0,0,0,17,
  	52,1,0,0,0,19,56,1,0,0,0,21,70,1,0,0,0,23,81,1,0,0,0,25,26,5,105,0,0,
  	26,27,5,110,0,0,27,28,5,116,0,0,28,2,1,0,0,0,29,30,5,109,0,0,30,31,5,
  	97,0,0,31,32,5,105,0,0,32,33,5,110,0,0,33,4,1,0,0,0,34,35,5,40,0,0,35,
  	6,1,0,0,0,36,37,5,41,0,0,37,8,1,0,0,0,38,39,5,123,0,0,39,10,1,0,0,0,40,
  	41,5,125,0,0,41,12,1,0,0,0,42,43,5,59,0,0,43,14,1,0,0,0,44,45,5,114,0,
  	0,45,46,5,101,0,0,46,47,5,116,0,0,47,48,5,117,0,0,48,49,5,114,0,0,49,
  	50,5,110,0,0,50,16,1,0,0,0,51,53,7,0,0,0,52,51,1,0,0,0,53,54,1,0,0,0,
  	54,52,1,0,0,0,54,55,1,0,0,0,55,18,1,0,0,0,56,57,5,47,0,0,57,58,5,42,0,
  	0,58,62,1,0,0,0,59,61,9,0,0,0,60,59,1,0,0,0,61,64,1,0,0,0,62,63,1,0,0,
  	0,62,60,1,0,0,0,63,65,1,0,0,0,64,62,1,0,0,0,65,66,5,42,0,0,66,67,5,47,
  	0,0,67,68,1,0,0,0,68,69,6,9,0,0,69,20,1,0,0,0,70,74,5,35,0,0,71,73,9,
  	0,0,0,72,71,1,0,0,0,73,76,1,0,0,0,74,75,1,0,0,0,74,72,1,0,0,0,75,77,1,
  	0,0,0,76,74,1,0,0,0,77,78,5,10,0,0,78,79,1,0,0,0,79,80,6,10,0,0,80,22,
  	1,0,0,0,81,82,7,1,0,0,82,83,1,0,0,0,83,84,6,11,1,0,84,24,1,0,0,0,4,0,
  	54,62,74,2,6,0,0,0,1,0
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
