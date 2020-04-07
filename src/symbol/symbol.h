#ifndef __SYM_SYMBOL_H_
#define __SYM_SYMBOL_H_

#include <iostream>
// #include "symtable.h"


#define sym_t unsigned int
#define NUM_SYM 31

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
    SYS_WRITE,      // write
};

class Symbol {
public:
    Symbol(sym_t symbolTag, int line, int offset, std::string val);
    virtual ~Symbol();

    sym_t       GetSymbolTag()   const;
    int         GetLine()        const;
    int         GetOffset()      const;
    std::string GetSymbolValue() const;

    virtual void Print(std::ostream& out=std::cout) const;

protected:
    sym_t symbolTag;
    int line;
    int offset;
    std::string symbolVal;

private:
    Symbol(const Symbol& t) = delete;
    bool operator=(const Symbol& t) const = delete;
};

#include "number.h"
#include "operator.h"
#include "word.h"

extern const sym_t _symbolMap[];
extern const std::string _symbolName[];
extern const std::string _keyWordName[];
extern const std::string _operatorName[];

#endif  //__SYM_SYMBOL_H_