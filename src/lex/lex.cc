#include "lex.h"

#include <iomanip>
#include "symbol.h"

void LexAnalyzer::Symbolize() {
    char cur;
    std::string val;
    Symbol* sym = nullptr;
    while((cur = NextChar()) != EOF) {
        if(cur == '\n') {
            NextLine();
            continue;
        }
        if(isspace(cur)) {
            continue;
        }
        val.clear();
        if(isdigit(cur)) {
            while(true) {
                val += cur;
                if((cur = NextChar()) == EOF) {
                    break;
                }
                if(!isdigit(cur)) {
                    PutBack(cur);
                    break;
                }
            }
            sym = new Sym_Number(line, offset - val.length(), val);
        } else if(isalpha(cur)) {
            while(true) {
                val += cur;
                if((cur = NextChar()) == EOF) {
                    break;
                }
                if(!isalnum(cur)) {
                    PutBack(cur);
                    break;
                }
            }
            sym = new Sym_Word(line, offset - val.length(), val);
        } else {
            val += cur;
            if((cur == ':' || cur == '<' || cur == '>')) {
                if((cur = NextChar()) == '=')   val += cur;
                else PutBack(cur);
            }
            sym = new Sym_Operator(line, offset - val.length(), val);
        }
        AppendSymbol(sym);
    }
}

int LexAnalyzer::AppendSymbol(Symbol* sym) {
    list.push_back(sym);
    return (int)list.size();
}

int LexAnalyzer::PrependSymbol(Symbol* sym) {
    list.push_front(sym);
    return (int)list.size();
}

Symbol* LexAnalyzer::GetSymbol() {
    Symbol* sym = nullptr;
    if(!list.empty())   sym = list.front(), list.pop_front();
    return sym;
}

void LexAnalyzer::ListSymbols(std::ostream&out) {
    out.setf(std::ios::right);
    out << std::endl 
        << std::setw(9) << "SYMVALUE"
        << "\t[SYMTAG:LINE:OFFSET]" 
        << std::endl;
    out << "------------------------------------------------------------" << std::endl;
    for(Symbol* sym:list)   sym->Print(out);
    out << "------------------------------------------------------------" << std::endl;
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