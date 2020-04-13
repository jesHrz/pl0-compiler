#include <iostream>
#include <fstream>
#include "lex.h"

std::ifstream fin;
LexAnalyzer* lex;

void Halt(int code=0) {
    delete lex;
    if(fin.is_open())   fin.close();
    if(code)    std::cerr << "pl0: exit with code " << code << std::endl;
    exit(code);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: pl0 <src>" << std::endl;
        return 0;
    }
    
    fin.open(argv[1]);
    lex = new LexAnalyzer(fin);
    lex->Symbolize();
    lex->ListSymbols();

    Halt();
}