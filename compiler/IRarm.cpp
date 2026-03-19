#include "IR.h"

// IR //

void IR::gen_arm(ostream &o) {
    for (const auto& entry : cfgsMap) {
        const string& functionName = entry.first;
        CFG* cfg = entry.second;
        if (cfg == nullptr) {
            continue;
        }

        cfg->gen_arm_prologue(o, functionName);
        cfg->gen_arm(o);
        cfg->gen_arm_epilogue(o);
    }

    this->currentCfg->gen_arm_prologue(o, "main");
    this->currentCfg->gen_arm(o);
    this->currentCfg->gen_arm_epilogue(o);
}

// CFG // 

int CFG::get_var_index_arm(string name){
    return -1 * this->SymbolIndex.at(name);
}

void CFG::gen_arm_prologue(ostream &o, const string& functionName){
    #ifdef __APPLE__
        o << "    .globl _" << functionName << "\n";
        o << "_" << functionName << ":\n";
    #else
        o << "    .globl " << functionName << "\n";
        o << functionName << ":\n";
    #endif

    int size = static_cast<int>(this->SymbolIndex.size()) * 4 + 4;

    // Il faut aligner avec un multiple de 16 pour arm
    this->stackSize = ((size + 15) / 16) * 16; 
    o << "    sub sp, sp, #" << stackSize << "\n";

    // Il faut mettre 0 dans la valeur retournée par défaut par return (au cas où)
    o << "    str wzr, [sp, #" << (stackSize - 4) << "]\n";
}

void CFG::gen_arm_epilogue(ostream &o){
    o << "    add sp, sp, #" << stackSize << "\n";
    o << "    ret\n";
}

// BasicBlock // 

void BasicBlock::gen_arm(ostream &o) {
    for (auto instr : this->instrs) {
        instr->gen_arm(o);
    }
}

void CFG::gen_arm(ostream &o) {
    for (auto bb : this->bbs) {
        bb->gen_arm(o);
    }
}


void IRInstr::gen_arm(ostream &o) {
    std::string nameVar1, nameVar2, nameVar3;
    int nb;
    int index1, index2, index3;
    switch(this->op) {
        case IRInstr::ldconst:
            // var1 = const

            nameVar1 = this->params.at(0);
            nb = stoi(this->params.at(1));

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);

            o << "    mov w8, #" << nb << "\n";
            o << "    str w8, [sp, #" << index1 << "]\n";
            break;
        case IRInstr::add:
            // var1 = var2 + var3

            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [sp, #" << index2 << "]\n";
            o << "    ldr w9, [sp, #" << index3 << "]\n";
            o << "    add w0, w8, w9\n";
            o << "    str w0, [sp, #" << index1 << "]\n";

            break;
        case IRInstr::sub:
            // var1 = var2 - var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [sp, #" << index2 << "]\n";
            o << "    ldr w9, [sp, #" << index3 << "]\n";
            o << "    subs w0, w8, w9\n";
            o << "    str w0, [sp, #" << index1 << "]\n";
            break;
        case IRInstr::copy:
            // var1 = var2
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            o << "    ldr w0, [sp, #" << index2 << "]" << endl;
            o << "    str w0, [sp, #" << index1 << "]" << endl;

            break;
        case IRInstr::rtrn:
            nameVar1 = this->params.at(0);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);

            // Il faut charger la valeur dans w0
            o << "    ldr w0, [sp, #" << index1 << "]\n";
            break;
        case IRInstr::neg:
            nameVar1 = this->params.at(0);
            index1 = this->bb->cfg->get_var_index_arm(nameVar1);

            // On a w0 = - w8 <=> w0 = 0 - w8
            o << "    ldr w8, [sp, #" << index1 <<"]" << endl;
            o << "    sub w0, wzr, w8" << endl;
            o << "    str w0, [sp, #" << index1 << "]" << endl;
            break;
        case IRInstr::mul:
            // var1 = var2 * var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [sp, #" << index2 << "]\n";
            o << "    ldr w9, [sp, #" << index3 << "]\n";
            o << "    mul w0, w8, w9\n";
            o << "    str w0, [sp, #" << index1 << "]\n";
            break;
        case IRInstr::div:
            // Forme : var1 = var2 / var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [sp, #" << index2 << "]\n";
            o << "    ldr w9, [sp, #" << index3 << "]\n";
            o << "    sdiv w0, w8, w9\n";
            o << "    str w0, [sp, #" << index1 << "]\n";
            break;
        default:
            break;
    }
}
