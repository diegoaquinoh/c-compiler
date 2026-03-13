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

// CFG //

void CFG::add_bb(BasicBlock* bb){
    if (bb == nullptr) {
        return;
    }

    bb->cfg = this;
    this->bbs.push_back(bb);
    this->current_bb = bb;
}

void CFG::add_to_symbol_table(string name, Type t){
    if (this->SymbolIndex.find(name) != this->SymbolIndex.end()) {
        return;
    }

    if (this->nextFreeSymbolIndex == 0) {
        this->nextFreeSymbolIndex = -4;
    }

    this->nextFreeSymbolIndex -= 4;
    this->SymbolType[name] = t;
    this->SymbolIndex[name] = this->nextFreeSymbolIndex;
}


string CFG::create_new_tempvar(Type t){

    int tempIndex = static_cast<int>(this->SymbolIndex.size());
    string tempName;
    tempName = "tmp" + to_string(tempIndex);

    while (this->SymbolIndex.find(tempName) != this->SymbolIndex.end()){
        ++tempIndex;
        tempName = "tmp" + to_string(tempIndex);
    }

    this->add_to_symbol_table(tempName, t);
    return tempName;
}

int CFG::get_var_index(string name){
    return this->SymbolIndex[name];
}

Type CFG::get_var_type(string name){
    return this->SymbolType[name];
}




// BasicBlock // 

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
    this->cfg = cfg;
    this->label = entry_label;
}
