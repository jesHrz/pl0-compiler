#ifndef __SYM_KEYWORD_H_
#define __SYM_KEYWORD_H_

#include "symbol.h"

class Sym_Word: public Symbol {
public:
    Sym_Word(int line, int offset, std::string val);
    ~Sym_Word();

private:
    Sym_Word(const Sym_Word& t) = delete;
    bool operator=(const Sym_Word& t) const = delete;

public:
    static sym_t GetWordSymbolTag(std::string word);
};

#endif // __SYM_KEYWORD_H_