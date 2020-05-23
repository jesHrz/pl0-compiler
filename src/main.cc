#include <fstream>
#include <iostream>
#include <unistd.h>

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

#ifdef VM

#include "vm/vm.h"

VirtualMachine *vm;
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

#ifdef VM
    delete vm;
#endif

    if (fin.is_open()) fin.close();
    if (exitCode)
        std::cerr << "pl0: exit with code " << exitCode << std::endl;
    else
        std::cout << "pl0: exit with code 0" << std::endl;
    exit(exitCode);
}

int main(int argc, char *argv[]) {

    signal(SIGINT, Halt);
    signal(SIGKILL, Halt);
    signal(SIGTERM, Halt);

    if (argc < 2) {
        std::cerr << "Usage: pl0 <src>" << std::endl;
        return 0;
    }
    fin.open(argv[1]);

#ifdef LEX
    lex = new LexAnalyzer(fin);
    lex->Symbolize();
//    lex->ListSymbols();
#endif

#ifdef GRAMMAR
    pcodes = new CodeTable;
    gram = new GrammarAnalyzer(lex, pcodes);
    gram->Program();
    gram->ListIdTable();
    pcodes->ListCodes();
#ifdef VM
    vm = new VirtualMachine(pcodes);
    vm->run();
#endif
#endif
    Halt(0);
}