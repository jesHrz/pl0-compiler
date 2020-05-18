#ifndef __LEX_H_
#define __LEX_H_

#include <iostream>
#include <list>

#include "symbol/symbol.h"

class LexAnalyzer {
public:
    explicit LexAnalyzer(std::istream &file = std::cin) : in(file), line(1), offset(0) {};

    void Symbolize();

    int AppendSymbol(Symbol *sym);

    int PrependSymbol(Symbol *sym);

    Symbol *GetSymbol();

    void ListSymbols(std::ostream &out = std::cout);

protected:
    std::istream &in;
    std::list<Symbol *> list;
    int line;
    int offset;

    void NextLine();

    void PutBack(char c);

    char NextChar();
};

#endif // __LEX_H_