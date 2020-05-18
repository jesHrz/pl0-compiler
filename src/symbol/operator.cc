#include "symbol/operator.h"


static const std::string operatorName[NUM_OP]{
        "+", "-", "*", "/", "=", "#", "<", "<=", ">", ">=", "(", ")", ",", ";", ":=", ".",
};

Sym_Operator::Sym_Operator(int line, int offset, const std::string &val) : Symbol(
        Sym_Operator::GetOperatorSymbolTag(val), line, offset, val) {}

Sym_Operator::~Sym_Operator() = default;

sym_t Sym_Operator::GetOperatorSymbolTag(const std::string &op) {
    for (int i = 0; i < NUM_OP; i++) {
        if (operatorName[i] == op) {
            return symbolMap[i + 3];
        }
    }
    return SYM_NUL;
}