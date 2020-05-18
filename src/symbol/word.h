#ifndef __SYM_KEYWORD_H_
#define __SYM_KEYWORD_H_

#include "symbol/symbol.h"

#define NUM_KEYWORD 13
#define MAX_KEYWORD_LEN 10

class Sym_Word : public Symbol {
public:
    Sym_Word(int line, int offset, const std::string &val);

    ~Sym_Word();

private:
    Sym_Word(const Sym_Word &t) = delete;

    bool operator=(const Sym_Word &t) const = delete;

public:
    static sym_t GetWordSymbolTag(const std::string &word);
};

#endif // __SYM_KEYWORD_H_