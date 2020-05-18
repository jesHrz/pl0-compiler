#include <iostream>
#include <vector>
#include <string>
#include "symbol/symbol.h"

extern void Halt(int);

void Error(const char *msg) {
    std::cerr << msg << std::endl;
    Halt(-1);
}

void Error(Symbol *sym, const char *msg) {
    std::cerr << sym->GetLine()
              << ":"
              << sym->GetOffset()
              << " error: "
              << msg
              << " \""
              << sym->GetSymbolValue()
              << "\"" << std::endl;
    Halt(1);
}

void Error(Symbol *sym, std::vector<sym_t> needs) {
    std::cerr << sym->GetLine()
              << ":"
              << sym->GetOffset()
              << " error: missing ";
    auto tag = needs.begin();
    std::cerr << symbolName[*tag++];
    while (tag != needs.end()) {
        std::cerr << " or " << symbolName[*tag++];
    }
    std::cerr << ", read symbol \"" << sym->GetSymbolValue() << "\"" << std::endl;
    Halt(1);
}