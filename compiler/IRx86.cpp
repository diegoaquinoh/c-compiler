#include "IR.h"

// IR //

void IR::gen_x86(ostream &o) {
    for (const auto& entry : cfgsMap) {
        CFG* cfg = entry.second;
        if (cfg == nullptr) {
            continue;
        }

        cfg->gen_x86_prologue(o);
        cfg->gen_x86(o);
        cfg->gen_x86_epilogue(o);
    }
}

// CFG // 

void CFG::gen_x86_prologue(ostream &o){
    string functionName = "main";
    if (!this->bbs.empty()) {
        functionName = this->bbs.front()->label;
    }

    #ifdef __APPLE__
        o << "    .globl _" << functionName << "\n";
        o << "_" << functionName << ":\n";
    #else
        o << "    .globl " << functionName << "\n";
        o << functionName << ":\n";
    #endif

    o << "    pushq %rbp\n";
    o << "    movq %rsp, %rbp\n";

    int stackSize = static_cast<int>(this->SymbolIndex.size()) * 4 + 4;
    if (stackSize > 0) {
        o << "    subq $" << stackSize << ", %rsp\n";
    }

    o << "    movl $0, -4(%rbp)\n";
}

void CFG::gen_x86_epilogue(ostream &o){
    o << "    movq %rbp, %rsp\n";
    o << "    popq %rbp\n";
    o << "    retq\n";
}

// BasicBlock // 

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
    IRInstr * nouvInstr = new IRInstr(this, op, t, params);
    this->instrs.push_back(nouvInstr);
}



void BasicBlock::gen_x86(ostream &o) {
    for (auto instr : this->instrs) {
        instr->gen_x86(o);
    }
}

void CFG::gen_x86(ostream &o) {

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