#include "IR.h"

// IR //

void IR::gen_x86(ostream &o) {
    for (const auto& entry : cfgsMap) {
        const string& functionName = entry.first;
        CFG* cfg = entry.second;
        if (cfg == nullptr) {
            continue;
        }

        cfg->gen_x86_prologue(o, functionName);
        cfg->gen_x86(o);
        cfg->gen_x86_epilogue(o);
    }
}

// CFG // 

void CFG::gen_x86_prologue(ostream &o, const string& functionName){
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

void BasicBlock::gen_x86(ostream &o) {
    for (auto instr : this->instrs) {
        instr->gen_x86(o);
    }
}

void CFG::gen_x86(ostream &o) {
    for (auto bb : this->bbs) {
        bb->gen_x86(o);
    }
}


void IRInstr::gen_x86(ostream &o) {
    std::string nameVar1, nameVar2, nameVar3;
    int nb;
    int index1, index2, index3;
    switch(this->op) {
        case IRInstr::ldconst:
            
            nameVar1 = this->params.at(0);
            nb = stoi(this->params.at(1));

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);

            o << "    movl $" << nb << ", " << index1 << "(%rbp)\n";
            break;
        case IRInstr::add:
            // var1 = var2 + var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    addl " << index3 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;

            break;
        case IRInstr::sub:
            // var1 = var2 - var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    subl " << index3 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::copy:
            // var1 = var2
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
        case IRInstr::rtrn:
            nameVar1 = this->params.at(0);

            index1 = this->bb->cfg->get_var_index(nameVar1);

            o << "    movl " << index1 << "(%rbp), %eax" << endl;
        default:
            break;
    }
}
