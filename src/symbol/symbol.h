#ifndef __SYM_SYMBOL_H_
#define __SYM_SYMBOL_H_

#include <iostream>

#define NUM_SYM 32
#define sym_t unsigned int
#define num_t int

enum SymbolTable {
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

class Symbol {
public:
    Symbol(sym_t symbolTag, int line, int offset, std::string val);

    virtual ~Symbol();

    sym_t GetSymbolTag() const;

    int GetLine() const;

    int GetOffset() const;

    std::string GetSymbolValue() const;

    num_t GetNumber() const;

    virtual void Print(std::ostream &out = std::cout) const;

protected:
    sym_t symbolTag;
    int line;
    int offset;
    std::string symbolVal;
    num_t number{};

public:
    Symbol(const Symbol &t) = delete;

    Symbol &operator=(const Symbol &t) const = delete;
};

#include "number.h"
#include "operator.h"
#include "word.h"

extern const sym_t symbolMap[];
extern const std::string symbolName[];

#endif  //__SYM_SYMBOL_H_