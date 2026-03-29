#include "IR.h"
#include <sstream>

IR::IR() {
    this->currentCfg = nullptr;
}

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

    this->SymbolType[name] = t;
    this->SymbolIndex[name] = this->nextFreeSymbolIndex;
    this->nextFreeSymbolIndex += (t == DoubleType) ? 2 : 1;
}

int CFG::get_var_frame_offset(string name){
    return this->SymbolIndex.at(name) * -8;
}

Type CFG::get_var_type(string name){
    return this->SymbolType.at(name);
}

string CFG::new_BB_name(){
    string bbName = "bb" + to_string(this->nextBBnumber);
    ++this->nextBBnumber;
    return bbName;
}

void CFG::push_break_target(BasicBlock* bb) {
    this->breakTargets.push_back(bb);
}

void CFG::pop_break_target() {
    if (!this->breakTargets.empty()) {
        this->breakTargets.pop_back();
    }
}

BasicBlock* CFG::get_break_target() const {
    if (this->breakTargets.empty()) {
        return nullptr;
    }
    return this->breakTargets.back();
}

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
    this->cfg = cfg;
    this->label = entry_label;
    this->exit_true = nullptr;
    this->exit_false = nullptr;
    this->has_return = false;
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
    IRInstr * nouvInstr = new IRInstr(this, op, t, params);
    this->instrs.push_back(nouvInstr);
}
void BasicBlock::add_IRInstr(IRInstr::Operation op) {
    IRInstr * nouvInstr = new IRInstr(this, op);
    this->instrs.push_back(nouvInstr);
}

static const char* opToString(IRInstr::Operation op) {
    switch (op) {
        case IRInstr::ldconst: return "ldconst";
        case IRInstr::copy:   return "copy";
        case IRInstr::add:    return "add";
        case IRInstr::sub:    return "sub";
        case IRInstr::mul:    return "mul";
        case IRInstr::div:    return "div";
        case IRInstr::mod:    return "mod";
        case IRInstr::neg:    return "neg";
        case IRInstr::lnot:   return "lnot";
        case IRInstr::rmem:   return "rmem";
        case IRInstr::wmem:   return "wmem";
        case IRInstr::itod:   return "itod";
        case IRInstr::dtoi:   return "dtoi";
        case IRInstr::call:   return "call";
        case IRInstr::bxor:   return "bxor";
        case IRInstr::bor:    return "bor";
        case IRInstr::band:   return "band";
        case IRInstr::cmp_eq: return "cmp_eq";
        case IRInstr::cmp_ne: return "cmp_ne";
        case IRInstr::cmp_lt: return "cmp_lt";
        case IRInstr::cmp_le: return "cmp_le";
        case IRInstr::cmp_gt: return "cmp_gt";
        case IRInstr::cmp_ge: return "cmp_ge";
        case IRInstr::rtrn:   return "rtrn";
        default:              return "<unknown>";
    }
}

string IRInstr::toString() const {
    ostringstream oss;
    oss << opToString(op);
    for (const auto &p : params) {
        oss << " " << p;
    }
    return oss.str();
}

string BasicBlock::toString() const {
    ostringstream oss;
    oss << "BB " << label << ":\n";
    for (auto instr : instrs) {
        oss << "  " << instr->toString() << "\n";
    }
    oss << "  exit_true=" << (exit_true ? exit_true->label : "null")
        << " exit_false=" << (exit_false ? exit_false->label : "null") << "\n";
    oss << "  has_return=" << (has_return ? "true" : "false") << "\n";
    if (!test_var_name.empty()) {
        oss << "  test_var=" << test_var_name << "\n";
    }
    return oss.str();
}

string CFG::toString() const {
    ostringstream oss;
    string name = "<unknown>";
    if (!bbs.empty()) {
        name = bbs.front()->label;
    }
    oss << "CFG " << name << ":\n";
    for (auto bb : bbs) {
        oss << bb->toString();
    }
    return oss.str();
}

string IR::toString() const {
    ostringstream oss;
    for (const auto &entry : cfgsMap) {
        oss << "CFG name=\"" << entry.first << "\"\n";
        if (entry.second) {
            oss << entry.second->toString();
        } else {
            oss << "  <null CFG>\n";
        }
    }
    return oss.str();
}

ostream& operator<<(ostream &o, const IR &ir) {
    o << ir.toString();
    return o;
}
