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


	IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params) : bb(bb_), op(op), t(t), params(params) {};
	IRInstr(BasicBlock* bb_, Operation op) : bb(bb_), op(op), t(Type::IntType), params({}) {};

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



class BasicBlock {
 public:
	BasicBlock(CFG* cfg, string entry_label);
	void gen_x86(ostream &o); /**< x86 assembly code generation for this basic block (very simple) */
	void gen_arm(ostream &o);
	string toString() const;

	void add_IRInstr(IRInstr::Operation op, Type t, vector<string> params);
	void add_IRInstr(IRInstr::Operation op);

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

class CFG {
 public:
	CFG(IR* ast) : ast(ast), nextFreeSymbolIndex(-4), nextBBnumber(0) {
		this->current_bb = nullptr;
	};

	IR* ast; /**< The AST this CFG comes from */
	
	void add_bb(BasicBlock* bb);

	// x86 code generation: could be encapsulated in a processor class in a retargetable compiler
	void gen_x86(ostream& o);
	string toString() const;
	string IR_reg_to_asm(string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
	void gen_x86_prologue(ostream& o, const string& functionName);
	void gen_x86_epilogue(ostream& o);

	// arm code generation
	void gen_arm(ostream& o);
	void gen_arm_prologue(ostream& o, const string& functionName);
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
