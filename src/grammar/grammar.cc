#include "grammar.h"
#include <vector>

extern void Error(const char*);
extern void Error(Symbol*, const char*);
extern void Error(Symbol*, std::vector<sym_t>);
GrammarAnalyzer::GrammarAnalyzer(LexAnalyzer* lex): lex(lex), sym(nullptr), level(0) {
    addr = new int[RECURSION_DEPTH];
    table = new std::map<std::string, Identifier>[RECURSION_DEPTH];
    for(int i = 0; i < RECURSION_DEPTH; ++i)    addr[i] = 0;
}

GrammarAnalyzer::~GrammarAnalyzer() {
    delete[] addr;
    delete[] table;
}

void GrammarAnalyzer::Getsym() {
    if(sym != nullptr) {
        delete sym;
        sym = nullptr;
    }
    sym = lex->GetSymbol();
    if(sym == nullptr) {
        // end of file
        Error("End of file, abort.");
    }
}

void GrammarAnalyzer::Retract() {
    lex->PrependSymbol(sym);
    sym = nullptr;
}

void GrammarAnalyzer::Program() {
    // std::cout << "program" << std::endl;
    Block();
    Getsym();
    if(sym->GetSymbolTag() != SYM_PERIOD) {
        // expect period. here
        Error(sym, {SYM_PERIOD});
    }
    std::cout << std::endl << "grammar analyze done." << std::endl;
}

void GrammarAnalyzer::Block() {
    // std::cout << "block" << std::endl;
    if(level >= RECURSION_DEPTH) {
        // recursion overflow
        Error("Too many recursions, abort.");
    }
    addr[level] += 3;
    table[level].clear();
    Decls();
    Stmts();
}

void GrammarAnalyzer::Decls() {
    // std::cout << "decls" << std::endl;
    while(true) {
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_CONST: Retract(); ConstDecl(); break;
        case SYM_VAR:   Retract(); VarDecl(); break;
        case SYM_PROC:  Retract(); ProcDecl(); break;
        default: Retract(); return;
        }
    }
}

void GrammarAnalyzer::ConstDecl() {
    // std::cout << "constdecl" << std::endl;
    Getsym();
    if(sym->GetSymbolTag() != SYM_CONST) {
        // expected CONST here
        Error(sym, {SYM_CONST});
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expect ident here
            Error(sym, {SYM_IDENT});
        }
        std::string constName = sym->GetSymbolValue();
        if(table[level].count(constName)) {
            // redeclaration
            Error(sym, "redefined identifier");
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_EQL) {
            // expect EQL= here
            Error(sym, {SYM_EQL});
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_NUMBER) {
            // expect NUMBER here
            Error(sym, {SYM_NUMBER});
        }
        table[level][constName] = Identifier(CONST, -1, -1, sym->GetNumber());
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_COMMA:     break;
        case SYM_SEMICOLON: return;
        default:            Error(sym, {SYM_COMMA, SYM_SEMICOLON});   // expect COMMA, or SEMICOLON; here
        }
    }
}

void GrammarAnalyzer::VarDecl() {
    // std::cout << "vardecl" << std::endl;
    Getsym();
    if(sym->GetSymbolTag() != SYM_VAR) {
        // expected VAR here
        Error(sym, {SYM_VAR});
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expect IDENT here
            Error(sym, {SYM_IDENT});
        }
        std::string varName = sym->GetSymbolValue();
        if(table[level].count(varName)) {
            Error(sym, "redefined identifier");
        }
        table[level][varName] = Identifier(VAR, level, addr[level]++, 0);
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_COMMA:     break;
        case SYM_SEMICOLON: return;
        default:            Error(sym, {SYM_COMMA, SYM_SEMICOLON});   // expect COMMA, or SEMICOLON; here
        }
    }
}

void GrammarAnalyzer::ProcDecl() {
    // std::cout << "procdecl" << std::endl;
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_PROC) {
            // expected PROCEDURE here
            Error(sym, {SYM_PROC});
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expect IDENT here
            Error(sym, {SYM_IDENT});
        }
        std::string procName = sym->GetSymbolValue();
        if(table[level].count(procName)) {
            Error(sym, "redefined identifier");
        }
        table[level][procName] = Identifier(PROC, level, addr[level]++, 0);
        Getsym();
        if(sym->GetSymbolTag() != SYM_SEMICOLON) {
            // expect SEMICOLON; here
            Error(sym, {SYM_SEMICOLON});
        }

        level++;
        Block();
        table[level--].clear();

        Getsym();
        if(sym->GetSymbolTag() != SYM_SEMICOLON) {
            // expect SEMICOLON; here
            Error(sym, {SYM_SEMICOLON});
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_PROC) {
            Retract();
            break;
        }
        Retract();
    }
}

void GrammarAnalyzer::Stmts() {
    Getsym();
    switch(sym->GetSymbolTag()) {
    case SYM_IDENT: Retract(); Assign();    break;  // 赋值语句
    case SYM_IF:    Retract(); Condition(); break; // 条件语句
    case SYM_WHILE: Retract(); Whiledo();   break; // 当型循环语句
    case SYM_CALL:  Retract(); Call();      break; // 过程调用语句
    case SYM_READ:  Retract(); Read();      break; // 读语句
    case SYM_WRITE: Retract(); Write();     break; // 写语句
    case SYM_BEGIN: Retract(); Compound();  break; // 复合语句
    default: Retract(); break;
    }
}

void GrammarAnalyzer::Assign() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_IDENT) {
        // expected IDENT here
        Error(sym, {SYM_IDENT});
    }
    Identifier* ident = FindIdentifier(sym->GetSymbolValue());
    if(!ident) {
        Error(sym, "undefined identifier");
    }
    if(ident->kind != VAR) {
        Error(sym, "identifier is not a var");
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_BECOMES) {
        // expect BECOME:= here
        Error(sym, {SYM_BECOMES});
    }
    Expr();
}

void GrammarAnalyzer::Compound() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_BEGIN) {
        // expected BEGIN here
        Error(sym, {SYM_BEGIN});
    }
    while(true) {
        Stmts();
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_SEMICOLON: break;
        case SYM_END:       return;
        default:            Error(sym, {SYM_SEMICOLON, SYM_END});    // epected SEMICOLON or END here
        }
    }
}

void GrammarAnalyzer::Cond() {
    Getsym();
    if(sym->GetSymbolTag() == SYM_ODD) {
        Expr();
        return;
    }
    Retract();
    Expr();
    Getsym();
    switch(sym->GetSymbolTag()) {
    case SYM_EQL:
    case SYM_NEQ:
    case SYM_LSS:
    case SYM_LEQ:
    case SYM_GTR:
    case SYM_GEQ:
        break;
    default:
        Error(sym, {SYM_EQL, SYM_NEQ, SYM_LSS, SYM_LEQ, SYM_GTR, SYM_GEQ}); // expected =,#,<,<=,>,>= here
    }
    Expr();
}

void GrammarAnalyzer::Expr() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_PLUS && sym->GetSymbolTag() != SYM_MINUS) {
        Retract();
    }
    while(true) {
        Term();
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_PLUS:
            break;
        case SYM_MINUS:
            break;
        default:
            Retract();
            return;
        }
    }
}

void GrammarAnalyzer::Term() {
    Factor();
    while(true) {
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_TIMES:
        case SYM_SLASH:
            break;
        default:
            Retract();
            return;
        }
        Factor();
    }
}

void GrammarAnalyzer::Factor() {
    Identifier* ident;
    Getsym();
    switch (sym->GetSymbolTag())
    {
    case SYM_IDENT:
        ident = FindIdentifier(sym->GetSymbolValue());
        if(!ident) {
            Error(sym, "undefined identifier");
        }
        if(ident->kind != VAR && ident->kind != CONST) {
            Error(sym, "identifier is not a var or const");
        }
        break;
    case SYM_NUMBER:
        break;
    case SYM_LPAREN:
        Expr();
        Getsym();
        if(sym->GetSymbolTag() != SYM_RPAREN) {
            // expected RPAREN) here
            Error(sym, {SYM_RPAREN});
        }
        break;
    default:
        Error(sym, {SYM_IDENT, SYM_NUMBER, SYM_LPAREN});    // nothing matched
    }
}

void GrammarAnalyzer::Condition() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_IF) {
        // expected IF here
        Error(sym, {SYM_IF});
    }
    Cond();
    Getsym();
    if(sym->GetSymbolTag() != SYM_THEN) {
        // expected THEN here
        Error(sym, {SYM_THEN});
    }
    Stmts();
}

void GrammarAnalyzer::Whiledo() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_WHILE) {
        // expected WHILE here
        Error(sym, {SYM_WHILE});
    }
    Cond();
    Getsym();
    if(sym->GetSymbolTag() != SYM_DO) {
        // expect DO here;
        Error(sym, {SYM_DO});
    }
    Stmts();
}

void GrammarAnalyzer::Call() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_CALL) {
        // expected CALL here
        Error(sym, {SYM_CALL});
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_IDENT) {
        // expected IDENT here
        Error(sym, {SYM_IDENT});
    }
    Identifier* ident = FindIdentifier(sym->GetSymbolValue());
    if(!ident) {
        Error(sym, "undefined indentifier");
    }
    if(ident->kind != PROC) {
        Error(sym, "identifier is not a procedure");
    }
}

void GrammarAnalyzer::Read() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_READ) {
        // expected READ here
        Error(sym, {SYM_READ});
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_LPAREN) {
        // expected LPAREN( here
        Error(sym, {SYM_LPAREN});
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expected IDENT here
            Error(sym, {SYM_IDENT});
        }
        Identifier* ident = FindIdentifier(sym->GetSymbolValue());
        if(!ident) {
            Error(sym, "undefined identifer");
        }
        if(ident->kind != VAR && ident->kind != CONST) {
            Error(sym, "identifier is not a var or const");
        }
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_COMMA:     break;
        case SYM_RPAREN:    return;
        default:            Error(sym, {SYM_COMMA, SYM_RPAREN});    // expected COMMA, or RPAREN) here
        }
    }
}

void GrammarAnalyzer::Write() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_WRITE) {
        // expected READ here
        Error(sym, {SYM_WRITE});
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_LPAREN) {
        // expected LPAREN( here
        Error(sym, {SYM_LPAREN});
    }
    while(true) {
        Expr();
        // Getsym();
        // if(sym->GetSymbolTag() != SYM_IDENT) {
        //     // expected IDENT here
        //     Error(sym, {SYM_IDENT});
        // }
        // Identifier* ident = FindIdentifier(sym->GetSymbolValue());
        // if(!ident) {
        //     Error(sym, "undefined identifer");
        // }
        // if(ident->kind != VAR && ident->kind != CONST) {
        //     Error(sym, "identifier is not a var or const");
        // }
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_COMMA:     break;
        case SYM_RPAREN:    return;
        default:            Error(sym, {SYM_COMMA, SYM_RPAREN});    // expected COMMA, or RPAREN) here
        }
    }
}