#ifndef __SYM_NUMBER_H_
#define __SYM_NUMBER_H_


#include "symbol.h"
#include <cmath>

#define MAX_NUMBER_POW int(ceil(log(std::numeric_limits<num_t>::max())/log(10)))

class Sym_Number: public Symbol {
public:
    Sym_Number(int line, int offset, std::string val);
    ~Sym_Number();

    void Print(std::ostream& out) const;

private:
    Sym_Number(const Sym_Number& t) = delete;
    bool operator=(const Sym_Number& t) const = delete;
};

#endif // __SYM_NUMBER_H_