#ifndef __SYM_OPERATOR_H_
#define __SYM_OPERATOR_H_

#include "symbol.h"
#define NUM_OP 15

class Sym_Operator: public Symbol {
public:
    Sym_Operator(int line, int offset, std::string val);
    ~Sym_Operator();

private:
    Sym_Operator(const Sym_Operator& t) = delete;
    bool operator=(const Sym_Operator& t) const = delete;

public:
    static sym_t GetOperatorSymbolTag(std::string op);
};

#endif // __SYM_OPERATOR_H_