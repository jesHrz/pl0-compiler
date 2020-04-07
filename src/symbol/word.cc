#include "word.h"
#include "symtable.h"

Sym_Word::Sym_Word(int line, int offset, std::string val): Symbol(Sym_Word::GetWordSymbolTag(val), line, offset, val) {};
Sym_Word::~Sym_Word() {};

sym_t Sym_Word::GetWordSymbolTag(std::string word)  {
    for(int i = 0; i < NUM_KEYWORD; i++) {
        if(_keyWordName[i] == word) {
            return _symbolMap[i + 3 + NUM_OP];
        }
    }
    return SYM_IDENT;
}