#include "IR.h"

// IR //

IR::IR() {
    this->cfg = nullptr;
}


// BasicBlock // 

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
    this->cfg = cfg;
    this->label = entry_label;
}