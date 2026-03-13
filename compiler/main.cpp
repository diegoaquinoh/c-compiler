#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"
#include "generated/ifccBaseVisitor.h"

#include "SymbolTableVisitor.h"
#include "IRGenVisitor.h"
#include "IR.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv)
{
  stringstream in;
  if (argn==2)
  {
     ifstream lecture(argv[1]);
     if( !lecture.good() )
     {
         cerr<<"error: cannot read file: " << argv[1] << endl ;
         exit(1);
     }
     in << lecture.rdbuf();
  }
  else
  {
      cerr << "usage: ifcc path/to/file.c" << endl ;
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
/*   SymbolTableVisitor stv;
  stv.visit(tree);

  if (stv.hasError()) {
      exit(1);
  }

  // Second Visitor: generate code using the symbol table
  IRGenVisitor v(stv.getSymbolTable());
  v.visit(tree); */

  // Temporaire : On définit une IR en dur pour générer le reste

// IR ir;
// CFG cfg(&ir);
// BasicBlock bb(&cfg, "entry_label1");
// bb.add_IRInstr(IRInstr::ldconst, IntType, {"var1", "4"});
// bb.add_IRInstr(IRInstr::ldconst, IntType, {"var2", "6"});
// bb.add_IRInstr(IRInstr::add, IntType, {"var3", "var1", "var2"});
// bb.gen_x86(std::cout);

IR ir;
CFG cfg(&ir);
string name = cfg.new_BB_name();
BasicBlock bb(&cfg, name);
bb.add_IRInstr(IRInstr::ldconst, IntType, {"var1", "4"});
bb.add_IRInstr(IRInstr::ldconst, IntType, {"var2", "6"});
bb.add_IRInstr(IRInstr::add, IntType, {"var3", "var1", "var2"});
cfg.add_bb(&bb);
ir.cfgsMap.insert({"main", &cfg});
ir.gen_x86(std::cout);



  return 0;
}
