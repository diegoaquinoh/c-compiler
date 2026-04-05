#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"
#include "generated/ifccBaseVisitor.h"

#include "SymbolTableVisitor.h"
#include "IRGenVisitor.h"
#include "IR.h"

using namespace antlr4;
using namespace std;

typedef enum { X86, ARM, IR } Architectures;
const std::map<std::string, Architectures> architecturesByName = {
    {"x86", X86},
    {"arm", ARM},
    {"ir", IR}
};

class StrictErrorListener : public antlr4::BaseErrorListener {
public:
    void syntaxError(antlr4::Recognizer*, antlr4::Token*, size_t line,
                     size_t col, const string &msg, exception_ptr) override {
        cerr << "error at line " << line << ":" << col << " " << msg << "\n";
        exit(1);
    }
};

int main(int argn, const char **argv)
{
    Architectures archiCible = X86;

    string outputPath;
    stringstream in;

    if (argn < 2 || argn > 5) {
        cerr << "usage: ifcc path/to/file.c [x86|arm|ir] [-o output_file]" << endl;
        exit(1);
    }

    const char* inputPath = argv[1];
    for (int i = 2; i < argn; ++i) {
        string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 >= argn) {
                cerr << "error: missing output file after -o" << endl;
                exit(1);
            }
            outputPath = argv[++i];
            continue;
        }

        auto itArchi = architecturesByName.find(arg);
        if (itArchi != architecturesByName.end()) {
            archiCible = itArchi->second;
            continue;
        }

        cerr << "error: unknown argument: " << arg << endl;
        cerr << "usage: ifcc path/to/file.c [x86|arm|ir] [-o output_file]" << endl;
        exit(1);
    }

    ifstream lecture(inputPath);
    if (!lecture.good()) {
        cerr << "error: cannot read file: " << inputPath << endl;
        exit(1);
    }
    in << lecture.rdbuf();
  
  ANTLRInputStream input(in.str());
  StrictErrorListener errorListener;

  ifccLexer lexer(&input);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&errorListener);

  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListener);
  tree::ParseTree* tree = parser.axiom();

  if(parser.getNumberOfSyntaxErrors() != 0)
  {
      cerr << "error: syntax error during parsing" << endl;
      exit(1);
  }

  // First Visitor: build symbol table and check for errors
  SymbolTableVisitor stv;
  stv.visit(tree);

  if (stv.hasError()) {
      exit(1);
  }

  // Second Visitor: generate code using the symbol table
  IRGenVisitor v;
  v.visit(tree);

  ostream* output = &cout;
  ofstream outFile;
  if (!outputPath.empty()) {
      outFile.open(outputPath);
      if (!outFile.good()) {
          cerr << "error: cannot write output file: " << outputPath << endl;
          exit(1);
      }
      output = &outFile;
  }

  switch (archiCible) {
    case X86:
        v.getIR().gen_x86(*output);
        break;
    case ARM:
        v.getIR().gen_arm(*output);
        break;
    case IR:
        *output << v.getIR().toString();
        break;
  }
  return 0;
}
