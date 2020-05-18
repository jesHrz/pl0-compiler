#include "symbol/word.h"

extern void Halt(int);

static const std::string keyWordName[NUM_KEYWORD] = {
        "const", "var", "procedure", "begin", "end", "odd", "if",
        "then", "call", "while", "do", "read", "write",
};

Sym_Word::Sym_Word(int line, int offset, const std::string &val) : Symbol(Sym_Word::GetWordSymbolTag(val), line, offset,
                                                                          val) {
    if (val.length() > MAX_NUMBER_POW) {
        std::cerr << symbolName[this->symbolTag]
                  << ":"
                  << line
                  << ":"
                  << offset
                  << ": error: \""
                  << val << "\" length is larger than " << MAX_KEYWORD_LEN << std::endl;
        Halt(1);
    }
};

Sym_Word::~Sym_Word() = default;

sym_t Sym_Word::GetWordSymbolTag(const std::string &word) {
    for (int i = 0; i < NUM_KEYWORD; i++) {
        if (keyWordName[i] == word) {
            return symbolMap[i + 3 + NUM_OP];
        }
    }
    return SYM_IDENT;
}

