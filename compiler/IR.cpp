#include "IR.h"

IR::IR() {
    this->cfg = nullptr;
}

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
    this->cfg = cfg;
    this->label = entry_label;
}


void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
    IRInstr * nouvInstr = new IRInstr(this, op, t, params);
    this->instrs.push_back(nouvInstr);
}

void IRInstr::gen_x86(ostream &o) {
    switch(this->op) {
        case IRInstr::ldconst:
            o << "ldconst";
            break;
        default:
    }
}

void BasicBlock::gen_x86(ostream &o) {
    for (auto instr : this->instrs) {
        instr->gen_x86(o);
    }
}

void CFG::gen_x86(ostream &o) {

}

void IR::gen_x86(ostream &o) {

}
