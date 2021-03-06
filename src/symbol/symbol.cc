#include "symbol/symbol.h"
#include <iomanip>
#include <utility>


const sym_t symbolMap[NUM_SYM] = {
        SYM_NUL,        // 空
        // variable
        SYM_IDENT,      // 标识符
        // number
        SYM_NUMBER,     // 数值
        // operator
        SYM_PLUS,       // +
        SYM_MINUS,      // -
        SYM_TIMES,      // *
        SYM_SLASH,      // /
        SYM_EQL,        // =
        SYM_NEQ,        // #
        SYM_LSS,        // <
        SYM_LEQ,        // <=
        SYM_GTR,        // >
        SYM_GEQ,        // >=
        SYM_LPAREN,     // (
        SYM_RPAREN,     // )
        SYM_COMMA,      // ,
        SYM_SEMICOLON,  // ;
        SYM_BECOMES,    // :=
        SYM_PERIOD,     // .
        // keyword
        SYM_CONST,      // const
        SYM_VAR,        // var
        SYM_PROC,       // procedure
        SYM_BEGIN,      // begin
        SYM_END,        // end
        SYM_ODD,        // odd
        SYM_IF,         // if
        SYM_THEN,       // then
        SYM_CALL,       // call
        SYM_WHILE,      // while
        SYM_DO,         // do
        SYM_READ,       // read
        SYM_WRITE,      // write
};
const std::string symbolName[NUM_SYM] = {
        "SYM_NUL", "SYM_IDENT", "SYM_NUMBER", "SYM_PLUS", "SYM_MINUS",
        "SYM_TIMES", "SYM_SLASH", "SYM_EQL", "SYM_NEQ", "SYM_LSS",
        "SYM_LEQ", "SYM_GTR", "SYM_GEQ", "SYM_LPAREN", "SYM_RPAREN",
        "SYM_COMMA", "SYM_SEMICOLON", "SYM_BECOMES", "SYM_PERIOD", "SYN_CONST",
        "SYN_VAR", "SYM_PROCEDURE", "SYM_BEGIN", "SYM_END", "SYM_ODD",
        "SYM_IF", "SYM_THEN", "SYM_CALL", "SYM_WHILE", "SYM_DO",
        "SYM_READ", "SYM_WRITE",
};

Symbol::Symbol(sym_t symbolTag, int line, int offset, std::string val) : symbolTag(symbolTag), line(line),
                                                                         offset(offset), symbolVal(std::move(val)) {}

Symbol::~Symbol() = default;

sym_t Symbol::GetSymbolTag() const { return symbolTag; };

int Symbol::GetLine() const { return line; };

int Symbol::GetOffset() const { return offset; }

std::string Symbol::GetSymbolValue() const { return symbolVal; };

num_t Symbol::GetNumber() const { return number; }

void Symbol::Print(std::ostream &out) const {
    out.setf(std::ios::right);
    out << std::setw(10) << this->symbolVal
        << "\t["
        << symbolName[this->symbolTag]
        << ":"
        << this->line
        << ":"
        << this->offset
        << "]" << std::endl;
};

