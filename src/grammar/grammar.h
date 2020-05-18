#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_


#include <map>
#include <string>
#include "lex.h"
#include "symbol.h"
#include "idtable.h"
#include "pcode.h"


#define RECURSION_DEPTH 3

class GrammarAnalyzer {
public:
    GrammarAnalyzer(LexAnalyzer* lex, CodeTable* pcodes);
    ~GrammarAnalyzer();

    void Getsym();
    void Retract();

    void Program();     // 程序

    void Block();       // 分程序
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

    void ListTable() const { mainTable->ListTable(); };

protected:
    LexAnalyzer* lex;
    Symbol *sym;
    CodeTable* pcodes;
    
    int level;
    int* addr;

    IdTable* mainTable;
    IdTable* curTable;
private:
    GrammarAnalyzer(const GrammarAnalyzer&)=delete;
    GrammarAnalyzer& operator=(const GrammarAnalyzer&) const = delete;

    Identifier* FindIdentifier(std::string name) {
        IdTable* table = this->curTable;
        while(table) {
            Identifier* ident = table->GetIdentifier(name);
            if(ident)   return ident;
            table = table->GetPreTable();
        }
        return nullptr;
    }
};

#endif // __GRAMMAR_H_