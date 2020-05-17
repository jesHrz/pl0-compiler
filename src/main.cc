#include <fstream>
#include <iostream>

#ifdef LEX
#include "lex/lex.h"
LexAnalyzer* lex;
#endif

#ifdef GRAMMAR
#include "grammar/grammar.h"
GrammarAnalyzer* gram;
#endif

#define PRINT_GRAMMAR_TREE 1

std::ifstream fin;

void Halt(int code=0) {
    #ifdef LEX
    delete lex;
    #endif

    #ifdef GRAMMAR
    delete gram;
    #endif

    if(fin.is_open())   fin.close();
    std::cerr << "pl0: exit with code " << code << std::endl;
    exit(code);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: pl0 <src>" << std::endl;
        return 0;
    }
    fin.open(argv[1]);
    // fin.open("/Users/jeshrz/Projects/pl0-compiler/test/code0.pl0");
    
    #ifdef LEX
    lex = new LexAnalyzer(fin);
    lex->Symbolize();
    // lex->ListSymbols();
    #endif

    #ifdef GRAMMAR
    gram = new GrammarAnalyzer(lex);
    gram->Program();
    gram->print();
    #endif
    Halt(0);
}