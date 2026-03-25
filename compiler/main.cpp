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

int main(int argn, const char **argv)
{
    Architectures archiCible = X86;
  stringstream in;
  if (argn<=3)
  {
     ifstream lecture(argv[1]);
     if( !lecture.good() )
     {
         cerr<<"error: cannot read file: " << argv[1] << endl ;
         exit(1);
     }
     in << lecture.rdbuf();
     if (argn == 3) {
        auto itArchi = architecturesByName.find(argv[2]);
        if (itArchi == architecturesByName.end()) {
            cerr << "usage: ifcc path/to/file.c [x86|arm|ir]" << endl ;
            exit(1);
        }

        archiCible = itArchi->second;
     }
  }
  else
  {
      cerr << "usage: ifcc path/to/file.c [x86|arm|ir]" << endl ;
      exit(1);
  }
  
  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
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

  switch (archiCible) {
    case X86:
        v.getIR().gen_x86(std::cout);
        break;
    case ARM:
        v.getIR().gen_arm(std::cout);
        break;
    case IR:
        cout << v.getIR().toString();
        break;
  }
  return 0;
}
