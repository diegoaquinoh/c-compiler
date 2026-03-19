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

// BasicBlock //

void BasicBlock::gen_x86(ostream &o) {
    // create a label for this block
    o << this->label << ":\n";

    // Emit instructions
    for (auto instr : this->instrs) {
        instr->gen_x86(o);
    }

    // Epilogue : we end execution
    if (this->exit_true == nullptr) {
        o << "    movq %rbp, %rsp\n";
        o << "    popq %rbp\n";
        o << "    retq\n";
        return;
    }

    // Test whether the block ended with a conditional, and if so, emit the appropriate jump
    // For while and ifs, we have a false exit that is not null
    if (this->exit_false !=  nullptr) {
        o << "    cmpl $0, " << " %eax" << "\n";
        o << "    je " << this->exit_false->label << "\n";
    }

    o << "    jmp " << this->exit_true->label << "\n";

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
            if(nameVar1 == "!reg")
            {
                // Keep the "!reg" pseudo-variable in sync with the register value.
                this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
                index1 = this->bb->cfg->get_var_index(nameVar1);

                o << "    movl $" << nb << ", %eax\n";
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            else {
                this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

                index1 = this->bb->cfg->get_var_index(nameVar1);

                o << "    movl $" << nb << ", " << index1 << "(%rbp)\n";
            }

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
        case IRInstr::copy: {
            // var1 = var2
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            bool destIsReg = (nameVar1 == "!reg");
            bool srcIsReg = (nameVar2 == "!reg");

            if (destIsReg) {
                if (!srcIsReg) {
                    index2 = this->bb->cfg->get_var_index(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax" << endl;
                }
                this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
                index1 = this->bb->cfg->get_var_index(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            } else {
                this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
                index1 = this->bb->cfg->get_var_index(nameVar1);

                if (srcIsReg) {
                    o << "    movl %eax, " << index1 << "(%rbp)" << endl;
                } else {
                    index2 = this->bb->cfg->get_var_index(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax" << endl;
                    o << "    movl %eax, " << index1 << "(%rbp)" << endl;
                }
            }
            break;
        }
        case IRInstr::rtrn:
            nameVar1 = this->params.at(0);

            index1 = this->bb->cfg->get_var_index(nameVar1);

            o << "    movl " << index1 << "(%rbp), %eax" << endl;
            break;
        case IRInstr::neg:
            nameVar1 = this->params.at(0);
            index1 = this->bb->cfg->get_var_index(nameVar1);
            o <<"    negl "<< index1 <<"(%rbp)\n";
            break;
        case IRInstr::mul:
            // var1 = var2 * var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    imull " << index3 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::div:
            // Forme : var1 = var2 / var3
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cltd" << endl;
            o << "    idivl " << index3 << "(%rbp)" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::bxor:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);
            // Forme : var1 = var2 ^ var3
            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    xorl " << index3 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::bor:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);
            // Forme : var1 = var2 | var3
            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    orl " << index3 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::band:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);
            // Forme : var1 = var2 & var3
            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    andl " << index3 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        default:
            break;
    }
}
