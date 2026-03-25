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
    #ifdef __APPLE__
        o << "    .globl _" << functionName << "\n";
        o << "_" << functionName << ":\n";
    #else
        o << "    .globl " << functionName << "\n";
        o << functionName << ":\n";
    #endif

    o << "    pushq %rbp\n";
    o << "    movq %rsp, %rbp\n";

    int stackSz = static_cast<int>(this->SymbolIndex.size()) * 4 + 4;
    int allocSize = (stackSz + 64 + 15) & ~15;
    if (allocSize > 0) {
        o << "    subq $" << allocSize << ", %rsp\n";
    }

    o << "    movl $0, -4(%rbp)\n";

    // Copy parameters from registers to stack slots
    const char* argRegs[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
    for (size_t i = 0; i < paramNames.size() && i < 6; i++) {
        int idx = get_var_index(paramNames[i]);
        o << "    movl " << argRegs[i] << ", " << idx << "(%rbp)\n";
    }
}

void CFG::gen_x86_epilogue(ostream &o){
    o << functionName << "_end:\n";
    o << "    movq %rbp, %rsp\n";
    o << "    popq %rbp\n";
    o << "    retq\n";
}

// BasicBlock // 

void BasicBlock::gen_x86(ostream &o) {
    // Output label for this basic block
    o << label << ":\n";

    // Generate all instructions
    for (auto instr : this->instrs) {
        instr->gen_x86(o);
    }

    // Generate branch logic
    if (exit_true == nullptr) {
        // End of function: jump to epilogue (handled by CFG)
    } else if (exit_false == nullptr) {
        // Unconditional jump
        o << "    jmp " << exit_true->label << "\n";
    } else {
        // Conditional branch: test_var_name != 0 → exit_true, else → exit_false
        int testIdx = cfg->get_var_index(test_var_name);
        o << "    cmpl $0, " << testIdx << "(%rbp)\n";
        o << "    je " << exit_false->label << "\n";
        o << "    jmp " << exit_true->label << "\n";
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
            // var1 = const
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

        case IRInstr::call: {
            string dest = this->params.at(0);
            string funcName = this->params.at(1);

            const char* argRegs[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

            for (size_t i = 2; i < this->params.size(); i++) {
                int argIndex = this->bb->cfg->get_var_index(this->params.at(i));
                o << "    movl " << argIndex << "(%rbp), " << argRegs[i - 2] << endl;
            }

            // Zero %eax (required by ABI for variadic/external functions)
            o << "    movl $0, %eax" << endl;

            #ifdef __APPLE__
                o << "    call _" << funcName << endl;
            #else
                o << "    call " << funcName << "@PLT" << endl;
            #endif

            // Store return value (%eax) into destination
            this->bb->cfg->add_to_symbol_table(dest, this->t);
            int destIndex = this->bb->cfg->get_var_index(dest);
            o << "    movl %eax, " << destIndex << "(%rbp)" << endl;
            break;
        }
        case IRInstr::copy:
            // var1 = var2
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
            this->bb->cfg->add_to_symbol_table(nameVar2, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::rtrn:
            nameVar1 = this->params.at(0);

            index1 = this->bb->cfg->get_var_index(nameVar1);

            o << "    movl " << index1 << "(%rbp), %eax" << endl;
            o << "    jmp " << this->bb->cfg->functionName << "_end" << endl;
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
        case IRInstr::mod:
            // Forme : var1 = var2 % var3
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
            o << "    movl %edx, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::lnot:
            // Forme : var1 = !var2
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);

            o << "    cmpl $0, " << index2 << "(%rbp)" << endl;
            o << "    sete %al" << endl;
            o << "    movzbl %al, %eax" << endl;
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
        case IRInstr::cmp_eq:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cmpl " << index3 << "(%rbp), %eax" << endl;
            o << "    sete  %al" << endl;
            o << "    movzbl %al, %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::cmp_ne:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cmpl " << index3 << "(%rbp), %eax" << endl;
            o << "    setne %al" << endl;
            o << "    movzbl %al, %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::cmp_lt:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cmpl " << index3 << "(%rbp), %eax" << endl;
            o << "    setl %al" << endl;
            o << "    movzbl %al, %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::cmp_le:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cmpl " << index3 << "(%rbp), %eax" << endl;
            o << "    setle %al" << endl;
            o << "    movzbl %al, %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::cmp_gt:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cmpl " << index3 << "(%rbp), %eax" << endl;
            o << "    setg %al" << endl;
            o << "    movzbl %al, %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        case IRInstr::cmp_ge:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index(nameVar1);
            index2 = this->bb->cfg->get_var_index(nameVar2);
            index3 = this->bb->cfg->get_var_index(nameVar3);

            o << "    movl " << index2 << "(%rbp), %eax" << endl;
            o << "    cmpl " << index3 << "(%rbp), %eax" << endl;
            o << "    setge %al" << endl;
            o << "    movzbl %al, %eax" << endl;
            o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            break;
        default:
            break;
    }
}
