#ifndef IR_H
#define IR_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>
using namespace std;
// Declarations from the parser -- replace with your own
#include "Type.h"
// #include "symbole.h"

class BasicBlock;
class CFG;
class DefFonction;
class IR;

//! The class for one 3-address instruction
class IRInstr {
 
   public:
	/** The instructions themselves -- feel free to subclass instead */
	typedef enum {
		ldconst,
		copy,
		add,
		sub,
		mul,
		div,
		mod,
		neg, // -(1) => neg var1 => var1 = -var1
		lnot,
		rmem,
		wmem,
		call,
		bxor,
		bor,
		band,
		cmp_eq,
		cmp_ne,
		cmp_lt,
		cmp_le,
		cmp_gt,
		cmp_ge,
		rtrn
	} Operation;


	/**  constructor */
	IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params) : bb(bb_), op(op), t(t), params(params) {};
	
	/** Actual code generation */
	void gen_x86(ostream &o); /**< x86 assembly code generation for this IR instruction */
	void gen_arm(ostream &o);
	string toString() const;
	
 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	Operation op;
	Type t;
	vector<string> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */
};






/**  The class for a basic block */

/* A few important comments.
	 IRInstr has no jump instructions.
	 cmp_* instructions behaves as an arithmetic two-operand instruction (add or mult),
	  returning a boolean value (as an int)

	 Assembly jumps are generated as follows:
	 BasicBlock::gen_x86() first calls IRInstr::gen_x86() on all its instructions, and then 
		    if  exit_true  is a  nullptr, 
            the epilogue is generated
        else if exit_false is a nullptr, 
          an unconditional jmp to the exit_true branch is generated
				else (we have two successors, hence a branch)
          an instruction comparing the value of test_var_name to true is generated,
					followed by a conditional branch to the exit_false branch,
					followed by an unconditional branch to the exit_true branch
	 The attribute test_var_name itself is defined when converting 
  the if, while, etc of the AST  to IR.

Possible optimization:
     a cmp_* comparison instructions, if it is the last instruction of its block, 
       generates an actual assembly comparison 
       followed by a conditional jump to the exit_false branch
*/

class BasicBlock {
 public:
	BasicBlock(CFG* cfg, string entry_label);
	void gen_x86(ostream &o); /**< x86 assembly code generation for this basic block (very simple) */
	void gen_arm(ostream &o);
	string toString() const;

	void add_IRInstr(IRInstr::Operation op, Type t, vector<string> params);

	// No encapsulation whatsoever here. Feel free to do better.
	BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */ 
	BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
	string label; /**< label of the BB, also will be the label in the generated code */
	CFG* cfg; /** < the CFG where this block belongs */
	vector<IRInstr*> instrs; /** < the instructions themselves. */
  	string test_var_name;  /** < when generating IR code for an if(expr) or while(expr) etc,
													 store here the name of the variable that holds the value of expr */
	bool has_return; /** < true when this block is terminated by a return */
 protected:

 
};




/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
	 The entry block is the one with the same label as the AST function name.
	   (it could be the first of bbs, or it could be defined by an attribute value)
	 The exit block is the one with both exit pointers equal to nullptr.
     (again it could be identified in a more explicit way)

 */
class CFG {
 public:
	CFG(IR* ast) : ast(ast), nextFreeSymbolIndex(0), nextBBnumber(0) {
		this->current_bb = nullptr;
	};

	IR* ast; /**< The AST this CFG comes from */

	void add_bb(BasicBlock* bb);

	// x86 code generation: could be encapsulated in a processor class in a retargetable compiler
	void gen_x86(ostream& o);
	string toString() const;
	string IR_reg_to_asm(string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
	void gen_x86_prologue(ostream& o);
	void gen_x86_epilogue(ostream& o);

	// arm code generation
	void gen_arm(ostream& o);
	void gen_arm_prologue(ostream& o);
	void gen_arm_epilogue(ostream& o);
	int get_var_index_arm(string name);

	// symbol table methods
	void add_to_symbol_table(string name, Type t);
	string create_new_tempvar(Type t);
	int get_var_index(string name);
	Type get_var_type(string name);

	// basic block management
	string new_BB_name();
	BasicBlock* current_bb;

	// function metadata
	string functionName;
	vector<string> paramNames;

 protected:
	map <string, Type> SymbolType; /**< part of the symbol table  */
	map <string, int> SymbolIndex; /**< part of the symbol table  */
	int stackSize = 0;
	int nextFreeSymbolIndex; /**< to allocate new symbols in the symbol table */
	int nextBBnumber; /**< just for naming */

	vector <BasicBlock*> bbs; /**< all the basic blocks of this CFG*/
};


class IR {
	public:
	IR();
	//~IR();
	IR(DefFonction* ast);
	CFG* currentCfg;
	map<string, CFG*> cfgsMap;

	void gen_x86(ostream& o); /**< x86 assembly code generation for this function */
	void gen_arm(ostream& o);
	string toString() const;
};

ostream& operator<<(ostream &o, const IR &ir);

#endif
