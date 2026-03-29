#ifndef IR_H
#define IR_H

#include <map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Type.h"

class BasicBlock;
class CFG;
class IR;

class IRInstr {
   public:
    typedef enum {
        ldconst,
        copy,
        add,
        sub,
        mul,
        div,
        mod,
        neg,
        lnot,
        itod,
        dtoi,
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

    IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params)
        : bb(bb_), op(op), t(t), params(params) {}
    IRInstr(BasicBlock* bb_, Operation op)
        : bb(bb_), op(op), t(Type::IntType), params({}) {}

    void gen_x86(ostream &o);
    void gen_arm(ostream &o);
    string toString() const;

 private:
    BasicBlock* bb;
    Operation op;
    Type t;
    vector<string> params;
};

class BasicBlock {
 public:
    BasicBlock(CFG* cfg, string entry_label);
    void gen_x86(ostream &o);
    void gen_arm(ostream &o);
    string toString() const;

    void add_IRInstr(IRInstr::Operation op, Type t, vector<string> params);
    void add_IRInstr(IRInstr::Operation op);

    BasicBlock* exit_true;
    BasicBlock* exit_false;
    string label;
    CFG* cfg;
    vector<IRInstr*> instrs;
    string test_var_name;
    bool has_return;
};

class CFG {
 public:
    CFG() : nextFreeSymbolIndex(1), nextBBnumber(0), current_bb(nullptr) {}

    void add_bb(BasicBlock* bb);

    void gen_x86(ostream& o);
    string toString() const;
    void gen_x86_prologue(ostream& o, const string& functionName);
    void gen_x86_epilogue(ostream& o);
    int get_var_index_x86(string name);

    void gen_arm(ostream& o);
    void gen_arm_prologue(ostream& o);
    void gen_arm_epilogue(ostream& o);
    int get_var_index_arm(string name);

    void add_to_symbol_table(string name, Type t);
    Type get_var_type(string name);

    string new_BB_name();
    BasicBlock* current_bb;
    void push_break_target(BasicBlock* bb);
    void pop_break_target();
    BasicBlock* get_break_target() const;

    string functionName;
    vector<string> paramNames;

 protected:
    map<string, Type> SymbolType;
    map<string, int> SymbolIndex;
    int stackSize = 0;
    int nextFreeSymbolIndex;
    int nextBBnumber;

    vector<BasicBlock*> bbs;
    vector<BasicBlock*> breakTargets;
};

class IR {
   public:
    IR();
    CFG* currentCfg;
    map<string, CFG*> cfgsMap;

    void gen_x86(ostream& o);
    void gen_arm(ostream& o);
    string toString() const;
};

ostream& operator<<(ostream &o, const IR &ir);

#endif
