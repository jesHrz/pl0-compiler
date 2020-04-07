#ifndef __LEX_H_
#define __LEX_H_

#include <iostream>
#include <list>

#include "symbol.h"

class LexAnalyzer {
public:
    LexAnalyzer(std::istream& file=std::cin): in(file), line(1), offset(0) {};
    ~LexAnalyzer() {};

    void Symbolize();
    int PutSymbol(Symbol* sym);
    Symbol* GetSymbol();
    void ListSymbols();

protected:
    std::istream& in;
    std::list<Symbol*> list;
    int line;
    int offset;

    void NextLine();
    void PutBack(char c);
    char NextChar();
};

#endif // __LEX_H_