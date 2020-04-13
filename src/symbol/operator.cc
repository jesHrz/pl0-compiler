#include "operator.h"


const std::string _operatorName[NUM_OP] {
    "+", "-", "*", "/", "=", "#", "<", "<=", ">", ">=", "(", ")", ",", ";", ":=", ".", 
};

Sym_Operator::Sym_Operator(int line, int offset, std::string val): Symbol(Sym_Operator::GetOperatorSymbolTag(val), line, offset, val) {}
Sym_Operator::~Sym_Operator() {}

sym_t Sym_Operator::GetOperatorSymbolTag(std::string op)  {
    for(int i = 0; i < NUM_OP; i++) {
        if(_operatorName[i] == op) {
            return _symbolMap[i + 3];
        }
    }
    return SYM_NUL;
}