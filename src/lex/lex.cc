#include "lex.h"
#include "symbol.h"

void LexAnalyzer::Symbolize() {
    Symbol* sym = nullptr;
    std::string val;
    bool atEOF = false;
    char cur;
    while((cur = NextChar()) != EOF) {
        if(cur == '\n') {
            NextLine();
            continue;
        }
        if(isspace(cur)) {
            continue;
        }
        if(isdigit(cur)) {
            val.clear();
            atEOF = false;
            do {
                val += cur;
                if((cur = NextChar()) == EOF) {
                    atEOF = true;
                    break;
                }
            } while(isdigit(cur));
            if(!atEOF)  PutBack(cur);
            sym = new Sym_Number(line, offset - val.length(), val);
        } else if(isalpha(cur)) {
            val.clear();
            atEOF = false;
            do {
                val += cur;
                if((cur = NextChar()) == EOF) {
                    atEOF = true;
                    break;
                }
            } while(isalnum(cur));
            if(!atEOF)  PutBack(cur);
            sym = new Sym_Word(line, offset - val.length(), val);
        } else {
            val.clear();
            val += cur;
            if((cur == ':' || cur == '<' || cur == '>') && !in.eof()) {
                if((cur = NextChar()) == '=')   val += cur;
                else PutBack(cur);
            }
            sym = new Sym_Operator(line, offset - val.length(), val);
        }
        PutSymbol(sym);
    }
}

int LexAnalyzer::PutSymbol(Symbol* sym) {
    list.push_back(sym);
    return (int)list.size();
}

Symbol* LexAnalyzer::GetSymbol() {
    Symbol* sym = nullptr;
    if(!list.empty())   sym = list.front(), list.pop_front();
    return sym;
}

void LexAnalyzer::ListSymbols() {
    for(Symbol* sym:list)   sym->Print();
}

void LexAnalyzer::NextLine() {
    line++;
    offset = 0;
}

void LexAnalyzer::PutBack(char c) {
    offset--;
    in.putback(c);
}

char LexAnalyzer::NextChar() {
    char tmp = EOF;
    if(!in.eof())   in.get(tmp), offset++;
    return tmp;
}