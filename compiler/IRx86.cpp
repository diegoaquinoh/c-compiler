#include "IR.h"
#include <cstdint>
#include <cstring>
#include <sstream>

static std::string double_to_hex_immediate(const std::string &literal) {
    double value = stod(literal);
    uint64_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    std::ostringstream oss;
    oss << "0x" << std::hex << bits;
    return oss.str();
}

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

    int stackSlots = this->nextFreeSymbolIndex + 2;
    // Les double sont sur 8 octets donc on aligne sur des cases mémoires de 8
    int stackSize = stackSlots * 8;
    int allocSize = (stackSize + 64 + 15) & ~15;
    if (allocSize > 0) {
        o << "    subq $" << allocSize << ", %rsp\n";
    }

    o << "    movl $0, -8(%rbp)\n";

    const char* intArgRegs[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
    const char* dblArgRegs[] = {"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5"};
    int intIdx = 0, dblIdx = 0;
    for (size_t i = 0; i < this->paramNames.size() && i < 6; i++) {
        int idx = this->get_var_index_x86(this->paramNames[i]);
        Type paramType = this->get_var_type(this->paramNames[i]);
        if (paramType == DoubleType) {
            o << "    movsd " << dblArgRegs[dblIdx++] << ", " << idx << "(%rbp)\n";
        } else {
            o << "    movl " << intArgRegs[intIdx++] << ", " << idx << "(%rbp)\n";
        }
    }
}

void CFG::gen_x86_epilogue(ostream &o){
    o << "    movq %rbp, %rsp\n";
    o << "    popq %rbp\n";
    o << "    retq\n";
}

int CFG::get_var_index_x86(string name){
    return this->SymbolIndex.at(name) * -8;
}

void BasicBlock::gen_x86(ostream &o) {
    o << this->label << ":\n";

    for (auto instr : this->instrs) {
        instr->gen_x86(o);
    }

    if (this->exit_true == nullptr) {
        o << "    movq %rbp, %rsp\n";
        o << "    popq %rbp\n";
        o << "    retq\n";
        return;
    }

    if (this->exit_false == nullptr) {
        o << "    jmp " << this->exit_true->label << "\n";
        return;
    }

    o << "    cmpl $0, %eax\n";
    o << "    je " << this->exit_false->label << "\n";
    o << "    jmp " << this->exit_true->label << "\n";
}

void CFG::gen_x86(ostream &o) {
    for (auto bb : this->bbs) {
        bb->gen_x86(o);
    }
}


void IRInstr::gen_x86(ostream &o) {
    string nameVar1, nameVar2, nameVar3;
    int index1, index2, index3;
    string operandeMemory;
    switch (this->op) {
        case IRInstr::ldconst:
            nameVar1 = this->params.at(0);
            if (this->t == DoubleType) {
                string imm = double_to_hex_immediate(this->params.at(1));
                o << "    movabsq $" << imm << ", %rax\n";
                o << "    movq %rax, %xmm0\n";
                if (nameVar1 != "!freg") {
                    this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                }
            } else {
                int nb = stoi(this->params.at(1));
                o << "    movl $" << nb << ", %eax\n";
                if (nameVar1 != "!reg") {
                    this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                }
            }
            break;

        case IRInstr::copy:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            if (this->t == DoubleType) {
                bool destIsReg = (nameVar1 == "!freg");
                bool srcIsReg = (nameVar2 == "!freg");

                if (destIsReg && srcIsReg) {
                    break;
                }
                if (destIsReg) {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                    break;
                }
                if (srcIsReg) {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                    break;
                }

                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                break;
            }

            {
                bool destIsReg = (nameVar1 == "!reg");
                bool srcIsReg = (nameVar2 == "!reg");

                if (destIsReg && srcIsReg) {
                    break;
                }
                if (destIsReg) {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                    break;
                }
                if (srcIsReg) {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                    break;
                }

                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                o << "    movl " << index2 << "(%rbp), %eax\n";
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;

        case IRInstr::itod:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            if (nameVar2 == "!reg") {
                o << "    cvtsi2sdl %eax, %xmm0\n";
            } else {
                index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                o << "    cvtsi2sdl " << index2 << "(%rbp), %xmm0\n";
            }
            if (nameVar1 != "!freg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
            }
            break;

        case IRInstr::dtoi:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            if (nameVar2 == "!freg") {
                o << "    cvttsd2sil %xmm0, %eax\n";
            } else {
                index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                o << "    cvttsd2sil %xmm0, %eax\n";
            }
            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;

        case IRInstr::add:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            
            index2 = this->bb->cfg->get_var_index_x86(nameVar2);

            if (this->t == DoubleType) {
                if (nameVar3 != "!freg") {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    o << "    movsd " << index3 << "(%rbp), %xmm0\n";
                }
                o << "    addsd " << index2 << "(%rbp), %xmm0\n";
                if (nameVar1 != "!freg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                }
            } else {
                if (nameVar3 != "!reg") {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    o << "    movl " << index3 << "(%rbp), %eax\n";
                }
                o << "    addl " << index2 << "(%rbp), %eax\n";
                if (nameVar1 != "!reg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                }
            }
            break;
        case IRInstr::mul:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index2 = this->bb->cfg->get_var_index_x86(nameVar2);

            if (this->t == DoubleType) {
                if (nameVar3 != "!freg") {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    o << "    movsd " << index3 << "(%rbp), %xmm0\n";
                }
                o << "    mulsd " << index2 << "(%rbp), %xmm0\n";
                if (nameVar1 != "!freg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                }
            } else {
                if (nameVar3 != "!reg") {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    o << "    movl " << index3 << "(%rbp), %eax\n";
                }
                o << "    imull " << index2 << "(%rbp), %eax\n";
                if (nameVar1 != "!reg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                }
            }
            break;
        case IRInstr::sub:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                
                o << "    subsd " << operandeMemory << ", %xmm0\n";

                if (nameVar1 != "!freg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                }
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %edx\n";
                    operandeMemory = "%edx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                
                o << "    subl " << operandeMemory << ", %eax\n";

                if (nameVar1 != "!reg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                }
            }
            break;
        case IRInstr::div:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }

                o << "    divsd " << operandeMemory << ", %xmm0\n";

                if (nameVar1 != "!freg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                }
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }

                o << "    cltd\n";
                o << "    idivl " << operandeMemory << "\n";

                if (nameVar1 != "!reg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                }
            }
            break;
        case IRInstr::mod:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            // On s'assure que le dénominateur est dans ecx peu importe si c'est reg ou pas
            if (nameVar3 == "!reg") {
                o << "    movl %eax, %ecx" << endl;
                operandeMemory = "%ecx";
            } else {
                index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                operandeMemory = to_string(index3) + "(%rbp)";
            }

            if (nameVar2 != "!reg") {
                index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                o << "    movl " << index2 << "(%rbp), %eax" << endl;
            }
            
            o << "    cltd" << endl;
            o << "    idivl " << operandeMemory << endl;

            // Le reste de la division (donc résultat du modulo) est dans %edx
            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %edx, " << index1 << "(%rbp)" << endl;
            } else {
                o << "    movl %edx, %eax\n";
            }
            break;
        case IRInstr::neg:
            nameVar1 = this->params.at(0);
            if (this->t == DoubleType) {
                if (nameVar1 != "!freg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd " << index1 << "(%rbp), %xmm0\n";
                }
                o << "    movabsq $0x8000000000000000, %rax\n";
                o << "    movq %rax, %xmm1\n";
                o << "    xorpd %xmm1, %xmm0\n";
                if (nameVar1 != "!freg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movsd %xmm0, " << index1 << "(%rbp)\n";
                }
            } else {
                if (nameVar1 != "!reg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl " << index1 << "(%rbp), %eax\n";
                }
                o << "    negl %eax\n";
                if (nameVar1 != "!reg") {
                    index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                    o << "    movl %eax, " << index1 << "(%rbp)\n";
                }
            }
            break;
        case IRInstr::lnot:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            if (this->t == DoubleType) {
                if (nameVar2 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                } else {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm2\n";
                }
                o << "    xorpd %xmm1, %xmm1\n";
                o << "    ucomisd %xmm1, %xmm2\n";
                o << "    sete %al\n";
                o << "    movzbl %al, %eax\n";
            } else {
                if (nameVar2 == "!reg") {
                    o << "    cmpl $0, %eax\n";
                } else {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    cmpl $0, " << index2 << "(%rbp)\n";
                }
                o << "    sete %al\n";
                o << "    movzbl %al, %eax\n";
            }
            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::bxor:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            index2 = this->bb->cfg->get_var_index_x86(nameVar2);
            if (nameVar3 != "!reg") {
                index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                o << "    movl " << index3 << "(%rbp), %eax" << endl;
            }

            o << "    xorl " << index2 << "(%rbp), %eax" << endl;

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            }
            break;
        case IRInstr::bor:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            index2 = this->bb->cfg->get_var_index_x86(nameVar2);
            if (nameVar3 != "!reg") {
                index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                o << "    movl " << index3 << "(%rbp), %eax" << endl;
            }

            o << "    orl " << index2 << "(%rbp), %eax" << endl;

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            }
            break;
        case IRInstr::band:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index2 = this->bb->cfg->get_var_index_x86(nameVar2);

            if (nameVar3 != "!reg") {
                index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                o << "    movl " << index3 << "(%rbp), %eax" << endl;
            }

            o << "    andl " << index2 << "(%rbp), %eax" << endl;

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)" << endl;
            }
            break;
        case IRInstr::cmp_eq:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }

                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                o << "    ucomisd " << operandeMemory << ", %xmm0\n";
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                o << "    cmpl " << operandeMemory << ", %eax\n";
            }
            o << "    sete %al\n";
            o << "    movzbl %al, %eax\n";

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::cmp_ne:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }

                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                o << "    ucomisd " << operandeMemory << ", %xmm0\n";
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                o << "    cmpl " << operandeMemory << ", %eax\n";
            }
            o << "    setne %al\n";
            o << "    movzbl %al, %eax\n";

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::cmp_lt:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                o << "    ucomisd " << operandeMemory << ", %xmm0\n";
                // Attention il faut utiliser setb et pas setl car comparaisons sur des floattants
                o << "    setb %al\n";
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                o << "    cmpl " << operandeMemory << ", %eax\n";
                o << "    setl %al\n";
            }
            o << "    movzbl %al, %eax\n";

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::cmp_le:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                o << "    ucomisd " << operandeMemory << ", %xmm0\n";
                // Attention il faut utiliser setbe et pas setle car comparaisons sur des floattants
                o << "    setbe %al\n";
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                o << "    cmpl " << operandeMemory << ", %eax\n";
                o << "    setle %al\n";
            }
            o << "    movzbl %al, %eax\n";

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::cmp_gt:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                o << "    ucomisd " << operandeMemory << ", %xmm0\n";
                // Attention il faut utiliser seta et pas setg car comparaisons sur des floattants
                o << "    seta %al\n";
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                o << "    cmpl " << operandeMemory << ", %eax\n";
                o << "    setg %al\n";
            }
            o << "    movzbl %al, %eax\n";

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::cmp_ge:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);
            if (this->t == DoubleType) {
                if (nameVar3 == "!freg") {
                    o << "    movapd %xmm0, %xmm2\n";
                    operandeMemory = "%xmm2";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!freg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movsd " << index2 << "(%rbp), %xmm0\n";
                }
                o << "    ucomisd " << operandeMemory << ", %xmm0\n";
                // Attention il faut utiliser setae et pas setge car comparaisons sur des floattants
                o << "    setae %al\n";
            } else {
                if (nameVar3 == "!reg") {
                    o << "    movl %eax, %ecx\n";
                    operandeMemory = "%ecx";
                } else {
                    index3 = this->bb->cfg->get_var_index_x86(nameVar3);
                    operandeMemory = to_string(index3) + "(%rbp)";
                }
                if (nameVar2 != "!reg") {
                    index2 = this->bb->cfg->get_var_index_x86(nameVar2);
                    o << "    movl " << index2 << "(%rbp), %eax\n";
                }
                o << "    cmpl " << operandeMemory << ", %eax\n";
                o << "    setge %al\n";
            }
            o << "    movzbl %al, %eax\n";

            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl %eax, " << index1 << "(%rbp)\n";
            }
            break;
        case IRInstr::call: {
            string dest = this->params.at(0);
            string funcName = this->params.at(1);
            const char* intArgRegs[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
            const char* dblArgRegs[] = {"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5"};
            int intIdx = 0, dblIdx = 0;

            for (size_t i = 2; i < this->params.size(); i++) {
                Type argT = this->bb->cfg->get_var_type(this->params.at(i));
                int argIndex = this->bb->cfg->get_var_index_x86(this->params.at(i));
                if (argT == DoubleType) {
                    o << "    movsd " << argIndex << "(%rbp), " << dblArgRegs[dblIdx++] << "\n";
                } else {
                    o << "    movl " << argIndex << "(%rbp), " << intArgRegs[intIdx++] << "\n";
                }
            }

            o << "    movl $0, %eax\n";
            #ifdef __APPLE__
                o << "    call _" << funcName << "\n";
            #else
                o << "    call " << funcName << "@PLT\n";
            #endif

            if (dest != "!reg") {
                this->bb->cfg->add_to_symbol_table(dest, this->t);
                int destIndex = this->bb->cfg->get_var_index_x86(dest);
                o << "    movl %eax, " << destIndex << "(%rbp)\n";
            }
            break;
        }

        case IRInstr::rtrn:
            nameVar1 = this->params.at(0);
            if (nameVar1 != "!reg") {
                index1 = this->bb->cfg->get_var_index_x86(nameVar1);
                o << "    movl " << index1 << "(%rbp), %eax\n";
            }
            break;

        default:
            break;
    }
}
