#include "number.h"

#include <iomanip>
#include <sstream>

extern void Halt(int code);

Sym_Number::Sym_Number(int line, int offset, std::string val): Symbol(SYM_NUMBER, line, offset, val) {
    if(val.length() >= MAX_NUMBER_POW) {
        std::cerr << _symbolName[SYM_NUMBER]
                  << ":"
                  << line
                  << ":"
                  << offset
                  << ": error: "
                  << val << " is too big" << std::endl;
        Halt(1);
    }
    std::istringstream tmp(val);
    tmp >> this->number;
}

Sym_Number::~Sym_Number() {}

void Sym_Number::Print(std::ostream& out) const {
    out.setf(std::ios::right);
    out << std::setw(10) << this->number
        << "\t["
        << _symbolName[this->symbolTag]
        << ":"
        << this->line
        << ":"
        << this->offset
        << "]" << std::endl;
}

// num_t Sym_Number::GetNumber() const { return this->value; };