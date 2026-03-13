#include "IR.h"

// IR //

IR::IR() {
    this->cfg = nullptr;
}

void IRInstr::gen_x86(ostream &o) {
    switch(this->op) {
        case IRInstr::ldconst:
            o << "ldconst";
            break;
        default:
    }
}


// BasicBlock // 

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
    this->cfg = cfg;
    this->label = entry_label;
}