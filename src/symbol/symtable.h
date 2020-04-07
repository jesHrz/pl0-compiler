#ifndef __SYMTABLE_H_
#define __SYMTABLE_H_

#include <string>

#define sym_t unsigned int

#define NUM_SYM 31
#define NUM_OP 15
#define NUM_KEYWORD 13

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

inline sym_t _symbolMap[NUM_SYM] = {
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

inline std::string _symbolName[NUM_SYM] = {
    "SYM_NUL",       "SYM_IDENT",     "SYM_NUMBER",  "SYM_PLUS",   "SYM_MINUS",
    "SYM_TIMES",     "SYM_SLASH",     "SYM_EQL",     "SYM_NEQ",    "SYM_LSS",
    "SYM_LEQ",       "SYM_GTR",       "SYM_GEQ",     "SYM_LPAREN", "SYM_RPAREN",
    "SYM_COMMA",     "SYM_SEMICOLON", "SYM_BECOMES", "SYN_CONST",  "SYN_VAR",
    "SYM_PROCEDURE", "SYM_BEGIN",     "SYM_END",     "SYM_ODD",    "SYM_IF",
    "SYM_THEN",      "SYM_CALL",      "SYM_WHILE",   "SYM_DO",     "SYM_READ",
    "SYM_WRITE",
};

inline std::string _keyWordName[NUM_KEYWORD] = {
    "const", "var",  "procedure", "begin", "end",  "odd",   "if",
    "then",  "call", "while",     "do",    "read", "write",
};

inline std::string _operatorName[NUM_OP] {
    "+", "-", "*", "/", "=", "#", "<", "<=", ">", ">=", "(", ")", ",", ";", ":=",
};

#endif // __SYMTABLE_H_