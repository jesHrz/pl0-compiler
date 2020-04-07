#include "number.h"
#include "symtable.h"

Sym_Number::Sym_Number(int line, int offset, std::string val): Symbol(SYM_NUMBER, line, offset, val) {
    if(val.length() >= MAX_NUMBER_POW) {
        exit(1);
    }
    std::istringstream tmp(val);
    tmp >> this->value;
}

Sym_Number::~Sym_Number() {}

void Sym_Number::Print(std::ostream& out) const {
    out << this->value
        << "\t\t["
        << _symbolName[this->symbolTag]
        << ":"
        << this->line
        << ":"
        << this->offset
        << "]" << std::endl;
}

number_t Sym_Number::GetNumber() const { return this->value; };