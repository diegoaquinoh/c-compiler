#include "IR.h"
#include <cstdint>
#include <cstring>

static void emitLoadImm32(ostream &o, const string &reg, int32_t value) {
    uint32_t u = static_cast<uint32_t>(value);
    uint16_t lo = static_cast<uint16_t>(u & 0xFFFFu);
    uint16_t hi = static_cast<uint16_t>((u >> 16) & 0xFFFFu);

    o << "    movz " << reg << ", #" << lo << "\n";
    if (hi != 0) {
        o << "    movk " << reg << ", #" << hi << ", lsl #16\n";
    }
}

static void emitLoadImm64(ostream &o, const string &reg, uint64_t value) {
    uint16_t p0 = static_cast<uint16_t>(value & 0xFFFFu);
    uint16_t p1 = static_cast<uint16_t>((value >> 16) & 0xFFFFu);
    uint16_t p2 = static_cast<uint16_t>((value >> 32) & 0xFFFFu);
    uint16_t p3 = static_cast<uint16_t>((value >> 48) & 0xFFFFu);

    o << "    movz " << reg << ", #" << p0 << "\n";
    if (p1 != 0) o << "    movk " << reg << ", #" << p1 << ", lsl #16\n";
    if (p2 != 0) o << "    movk " << reg << ", #" << p2 << ", lsl #32\n";
    if (p3 != 0) o << "    movk " << reg << ", #" << p3 << ", lsl #48\n";
}

void IR::gen_arm(ostream &o) {
    for (const auto& entry : cfgsMap) {
        CFG* cfg = entry.second;
        if (cfg == nullptr) {
            continue;
        }

        cfg->gen_arm_prologue(o);
        cfg->gen_arm(o);
        cfg->gen_arm_epilogue(o);
    }
}

int CFG::get_var_index_arm(string name){
    // Keep 8-byte slots so frame offsets match the IR-level offset arithmetic.
    return -8 * this->SymbolIndex.at(name);
}

void CFG::gen_arm_prologue(ostream &o){
    #ifdef __APPLE__
        o << "    .globl _" << functionName << "\n";
        o << "_" << functionName << ":\n";
    #else
        o << "    .globl " << functionName << "\n";
        o << functionName << ":\n";
    #endif

    // Save frame pointer and link register
    o << "    stp x29, x30, [sp, #-16]!\n";
    o << "    mov x29, sp\n";

    // Reserve internal accumulators used by IR lowering.
    this->add_to_symbol_table("!reg", IntType);
    this->add_to_symbol_table("!freg", DoubleType);

    int size = this->nextFreeSymbolIndex * 8;
    // Add padding for temp variables created during codegen
    size += 64;

    // Align to 16 bytes for ARM
    this->stackSize = ((size + 15) / 16) * 16;
    o << "    sub sp, sp, #" << stackSize << "\n";

    // Copy parameters from registers to stack slots
    const char* intArgRegs[] = {"w0", "w1", "w2", "w3", "w4", "w5"};
    const char* dblArgRegs[] = {"d0", "d1", "d2", "d3", "d4", "d5"};
    int intIdx = 0, dblIdx = 0;
    for (size_t i = 0; i < paramNames.size() && i < 6; i++) {
        int idx = get_var_index_arm(paramNames[i]);
        Type paramType = get_var_type(paramNames[i]);
        if (paramType == DoubleType) {
            o << "    str " << dblArgRegs[dblIdx++] << ", [x29, #" << idx << "]\n";
        } else {
            o << "    str " << intArgRegs[intIdx++] << ", [x29, #" << idx << "]\n";
        }
    }
}

void CFG::gen_arm_epilogue(ostream &o){
    o << functionName << "_end:\n";
    o << "    add sp, sp, #" << stackSize << "\n";
    // Restore frame pointer and link register
    o << "    ldp x29, x30, [sp], #16\n";
    o << "    ret\n";
}

void BasicBlock::gen_arm(ostream &o) {
    o << label << ":\n";

    for (auto instr : this->instrs) {
        instr->gen_arm(o);
    }

    if (exit_true == nullptr) {
    } else if (exit_false == nullptr) {
        o << "    b " << exit_true->label << "\n";
    } else {
        int testIdx = cfg->get_var_index_arm(test_var_name);
        o << "    ldr w8, [x29, #" << testIdx << "]\n";
        o << "    cbz w8, " << exit_false->label << "\n";
        o << "    b " << exit_true->label << "\n";
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
            nameVar1 = this->params.at(0);
            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
            index1 = this->bb->cfg->get_var_index_arm(nameVar1);

            if (this->t == DoubleType) {
                double value = stod(this->params.at(1));
                uint64_t bits = 0;
                std::memcpy(&bits, &value, sizeof(bits));
                emitLoadImm64(o, "x8", bits);
                o << "    fmov d0, x8\n";
                o << "    str d0, [x29, #" << index1 << "]\n";
            } else {
                nb = stoi(this->params.at(1));
                emitLoadImm32(o, "w8", nb);
                o << "    str w8, [x29, #" << index1 << "]\n";
            }
            break;
        case IRInstr::add:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    add w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";

            break;
        case IRInstr::sub:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::copy:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
            this->bb->cfg->add_to_symbol_table(nameVar2, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            o << "    ldr w0, [x29, #" << index2 << "]" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;

            break;
        case IRInstr::rtrn:
            nameVar1 = this->params.at(0);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);

            o << "    ldr w0, [x29, #" << index1 << "]\n";
            o << "    b " << this->bb->cfg->functionName << "_end\n";
            break;
        case IRInstr::neg:
            nameVar1 = this->params.at(0);
            index1 = this->bb->cfg->get_var_index_arm(nameVar1);

            // Negation is lowered as 0 - operand.
            o << "    ldr w8, [x29, #" << index1 <<"]" << endl;
            o << "    sub w0, wzr, w8" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        case IRInstr::mul:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    mul w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::div:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    sdiv w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::mod:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            // On utilise le fait que : (c'est ce que fait gcc)
            // n = p * q +r avec :
            // a = p // q (div entière)
            // b = a * q (= n - r)
            // r = p - b
            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]" << endl;
            o << "    sdiv w10, w8, w9" << endl;
            o << "    mul w10, w10, w9" << endl;
            o << "    sub w0, w8, w10" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;

            break;
        case IRInstr::lnot:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            // On fait comme gcc, on utilise le fait que subs va positionner le flag Z si le résultat est nul 
            // eq est un test pour savoir si z == 1
            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    subs w8, w8, #0" << endl;
            o << "    cset w0, eq" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;

            break;
        case IRInstr::call: {
            string dest = this->params.at(0);
            string funcCallName = this->params.at(1);

            const char* intCallArgRegs[] = {"w0", "w1", "w2", "w3", "w4", "w5"};
            const char* dblCallArgRegs[] = {"d0", "d1", "d2", "d3", "d4", "d5"};
            int intIdx = 0, dblIdx = 0;

            for (size_t i = 2; i < this->params.size(); i++) {
                Type argT = this->bb->cfg->get_var_type(this->params.at(i));
                int argIndex = this->bb->cfg->get_var_index_arm(this->params.at(i));
                if (argT == DoubleType) {
                    o << "    ldr " << dblCallArgRegs[dblIdx++] << ", [x29, #" << argIndex << "]\n";
                } else {
                    o << "    ldr " << intCallArgRegs[intIdx++] << ", [x29, #" << argIndex << "]\n";
                }
            }

            #ifdef __APPLE__
                o << "    bl _" << funcCallName << "\n";
            #else
                o << "    bl " << funcCallName << "\n";
            #endif

            this->bb->cfg->add_to_symbol_table(dest, this->t);
            int destIndex = this->bb->cfg->get_var_index_arm(dest);
            if (this->t == DoubleType) {
                o << "    str d0, [x29, #" << destIndex << "]\n";
            } else {
                o << "    str w0, [x29, #" << destIndex << "]\n";
            }
            break;
        }
        case IRInstr::itod:
            // var1 (double) = (double) var2 (int)
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    scvtf d0, w8\n";
            o << "    str d0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::dtoi:
            // var1 (int) = (int) var2 (double)
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            o << "    ldr d0, [x29, #" << index2 << "]\n";
            o << "    fcvtzs w8, d0\n";
            o << "    str w8, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::wmem:
            // *(x29 + offsetVar) = valueVar
            nameVar1 = this->params.at(0); // offset
            nameVar2 = this->params.at(1); // value

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            o << "    ldrsw x8, [x29, #" << index1 << "]\n";
            o << "    add x8, x29, x8\n";
            if (this->t == DoubleType) {
                o << "    ldr d0, [x29, #" << index2 << "]\n";
                o << "    str d0, [x8]\n";
            } else {
                o << "    ldr w0, [x29, #" << index2 << "]\n";
                o << "    str w0, [x8]\n";
            }
            break;
        case IRInstr::rmem:
            // var1 = *(x29 + offsetVar)
            nameVar1 = this->params.at(0); // destination
            nameVar2 = this->params.at(1); // offset

            this->bb->cfg->add_to_symbol_table(nameVar1, this->t);
            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);

            o << "    ldrsw x8, [x29, #" << index2 << "]\n";
            o << "    add x8, x29, x8\n";
            if (this->t == DoubleType) {
                o << "    ldr d0, [x8]\n";
                o << "    str d0, [x29, #" << index1 << "]\n";
            } else {
                o << "    ldr w0, [x8]\n";
                o << "    str w0, [x29, #" << index1 << "]\n";
            }
            break;
        case IRInstr::bxor:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);
            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    eor w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::bor:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);
            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    orr w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::band:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);
            o << "    ldr w8, [x29, #" << index2 << "]\n";
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    and w0, w8, w9\n";
            o << "    str w0, [x29, #" << index1 << "]\n";
            break;
        case IRInstr::cmp_eq:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);
            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w8, w8, w9" << endl;
            o << "    cset w0, eq" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        case IRInstr::cmp_ne:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);
            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w8, w8, w9" << endl;
            o << "    cset w0, ne" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        case IRInstr::cmp_lt:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w8, w8, w9" << endl;
            o << "    cset w0, lt" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        case IRInstr::cmp_le:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w8, w8, w9" << endl;
            o << "    cset w0, le" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        case IRInstr::cmp_gt:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w8, w8, w9" << endl;
            o << "    cset w0, gt" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        case IRInstr::cmp_ge:
            nameVar1 = this->params.at(0);
            nameVar2 = this->params.at(1);
            nameVar3 = this->params.at(2);

            index1 = this->bb->cfg->get_var_index_arm(nameVar1);
            index2 = this->bb->cfg->get_var_index_arm(nameVar2);
            index3 = this->bb->cfg->get_var_index_arm(nameVar3);

            o << "    ldr w8, [x29, #" << index2 << "]" << endl;
            o << "    ldr w9, [x29, #" << index3 << "]\n";
            o << "    subs w8, w8, w9" << endl;
            o << "    cset w0, ge" << endl;
            o << "    str w0, [x29, #" << index1 << "]" << endl;
            break;
        default:
            break;
    }
}
