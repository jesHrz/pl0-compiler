#include <fstream>
#include "lex.h"

int main() {
    std::ifstream fin("../../test/code.pl0");
    LexAnalyzer* lex = new LexAnalyzer(fin);
    lex->Symbolize();
    lex->ListSymbols();
    delete lex;
    fin.close();
    return 0;
}