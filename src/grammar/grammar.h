#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_


#include <map>
#include <string>
#include "lex.h"
#include "symbol.h"


#define RECURSION_DEPTH 3


enum KIND {
    NIL,
    CONST,
    VAR,
    PROC,
};

class Token {
public:
    Token(enum KIND kind=NIL, int level=-1, int addr=-1, num_t value=0): kind(kind), level(level), addr(addr), value(value) {}
    Token(const Token& t): kind(t.kind), level(t.level), addr(t.addr), value(t.value) {}
    Token& operator=(const Token& t) {
        if(this == &t)  return *this;
        kind = t.kind;
        level = t.level;
        addr = t.addr;
        value = t.value;
        return *this;
    };

    KIND kind;
    int level;
    int addr;
    num_t value;
};

class GrammarAnalyzer {
public:
    GrammarAnalyzer(LexAnalyzer* lex);
    ~GrammarAnalyzer();

    void Getsym();

    void Program();     // 程序

    void Block();       // 分程序
    void Decls();       // 说明部分
    void Stmts();       // 语句

    void ConstDecl();   // 常量说明
    void VarDecl();     // 变量说明
    void ProcDecl();    // 过程说明

    void Assign();      // 赋值语句
    void Condition();   // 条件语句
    void Whiledo();     // 当型循环语句
    void Call();        // 过程调用语句
    void Read();        // 读语句
    void Write();       // 写语句
    void Compound();    // 复合语句

    void Cond();        // 条件
    void Expr();        // 表达式
    void Term();        // 项
    void Factor();      // 因子

protected:
    LexAnalyzer* lex;
    Symbol *sym;
    
    int level;
    int* addr;
    std::map<std::string, Token>* table;
private:
    GrammarAnalyzer(const GrammarAnalyzer&)=delete;
    GrammarAnalyzer& operator=(const GrammarAnalyzer&) const = delete;

    Token* FindToken(std::string name) {
        for(int i = level; i >= 0; --i) {
            if(table[i].count(name))    return &table[i][name];
        }
        return nullptr;
    }
};

#endif // __GRAMMAR_H_