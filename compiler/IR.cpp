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
    std::string nameVar;
    int nb;
    switch(this->op) {
        case IRInstr::ldconst:
            
            nameVar = this->params.at(0);
            nb = stoi(this->params.at(1));
            o << "ldconst " << nameVar << " = " << nb;
            break;
        case IRInstr::add:
            o << "";
            break;
        case IRInstr::rtrn: 
            o << "";
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

void CFG::add_to_symbol_table(string name, Type t) {

}