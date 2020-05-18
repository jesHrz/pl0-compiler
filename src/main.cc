#include <fstream>
#include <iostream>

#ifdef LEX

#include "lex/lex.h"

LexAnalyzer *lex;
#endif

#ifdef GRAMMAR

#include "grammar/grammar.h"
#include "grammar/pcode.h"

GrammarAnalyzer *gram;
CodeTable *pcodes;
#endif

std::ifstream fin;

void Halt(int exitCode = 0) {
#ifdef LEX
    delete lex;
#endif

#ifdef GRAMMAR
    delete gram;
    delete pcodes;
#endif

    if (fin.is_open()) fin.close();
    if (exitCode)
        std::cerr << "pl0: exit with code " << exitCode << std::endl;
    else
        std::cout << "pl0: exit with code 0" << std::endl;
    exit(exitCode);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: pl0 <src>" << std::endl;
        return 0;
    }
    fin.open(argv[1]);

#ifdef LEX
    lex = new LexAnalyzer(fin);
    lex->Symbolize();
#endif

#ifdef GRAMMAR
    pcodes = new CodeTable;
    gram = new GrammarAnalyzer(lex, pcodes);
    gram->Program();
    pcodes->ListCodes();
#endif
    Halt(0);
}