#include "symbol.h"

#include <iomanip>
#include "symtable.h"

Symbol::Symbol(sym_t symbolTag, int line, int offset, std::string val): symbolTag(symbolTag), line(line), offset(offset), symbolVal(val) {}
Symbol::~Symbol() {}

sym_t       Symbol::GetSymbolTag()      const { return symbolTag; };
int         Symbol::GetLine()           const { return line; };
int         Symbol::GetOffset()         const { return offset; }
std::string Symbol::GetSymbolValue()    const { return symbolVal; };

void Symbol::Print(std::ostream& out) const {
    out.setf(std::ios::right);
    out << std::setw(9) << this->symbolVal
        << "\t["
        << _symbolName[this->symbolTag]
        << ":"
        << this->line
        << ":"
        << this->offset
        << "]" << std::endl;
};