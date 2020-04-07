#ifndef __SYM_SYMBOL_H_
#define __SYM_SYMBOL_H_

#include <iostream>
#include "symtable.h"

class Symbol {
public:
    Symbol(sym_t symbolTag, int line, int offset, std::string val);
    virtual ~Symbol();

    sym_t       GetSymbolTag()   const;
    int         GetLine()        const;
    int         GetOffset()      const;
    std::string GetSymbolValue() const;

    virtual void Print(std::ostream& out=std::cout) const;

protected:
    sym_t symbolTag;
    int line;
    int offset;
    std::string symbolVal;

private:
    Symbol(const Symbol& t) = delete;
    bool operator=(const Symbol& t) const = delete;
};

#include "number.h"
#include "operator.h"
#include "word.h"

#endif  //__SYM_SYMBOL_H_