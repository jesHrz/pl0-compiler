#include "grammar.h"

extern void Halt(int);
GrammarAnalyzer::GrammarAnalyzer(LexAnalyzer* lex): lex(lex), sym(nullptr), level(0) {
    addr = new int[RECURSION_DEPTH];
    table = new std::map<std::string, Token>[RECURSION_DEPTH];
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
}

void GrammarAnalyzer::Program() {
    Block();
    Getsym();
    if(sym->GetSymbolTag() != SYM_PERIOD) {
        // expect period. here
        Halt(1);
    }
}

void GrammarAnalyzer::Block() {
    addr[level] += 3;
    Decls();
    Stmts();
}

void GrammarAnalyzer::Decls() {
    while(true) {
        Getsym();
        lex->PrependSymbol(sym);
        switch(sym->GetSymbolTag()) {
        case SYM_CONST: ConstDecl(); break;
        case SYM_VAR: VarDecl(); break;
        case SYM_PROC: ProcDecl(); break;
        default: return;
        }
    }
}

void GrammarAnalyzer::ConstDecl() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_CONST) {
        // expected CONST here
        Halt(1);
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expect ident here
            Halt(1);
        }
        std::string constName = sym->GetSymbolValue();
        if(table[level].count(constName)) {
            // redeclaration
            Halt(1);
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_EQL) {
            // expect EQL= here
            Halt(1);
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_NUMBER) {
            // expect NUMBER here
            Halt(1);
        }
        table[level][constName] = Token(CONST, -1, -1, sym->GetNumber());
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_COMMA: break;
        case SYM_SEMICOLON: return;
        default: Halt(1);   // expect COMMA, or SEMICOLON; here
        }
    }
}

void GrammarAnalyzer::VarDecl() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_VAR) {
        // expected VAR here
        Halt(1);
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expect IDENT here
            Halt(1);
        }
        table[level][sym->GetSymbolValue()] = Token(VAR, level, addr[level]++, 0);
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_COMMA: break;
        case SYM_SEMICOLON: return;
        default: Halt(1);   // expect COMMA, or SEMICOLON; here
        }
    }
}

void GrammarAnalyzer::ProcDecl() {
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_PROC) {
            // expected PROCEDURE here
            Halt(1);
        }
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expect IDENT here
            Halt(1);
        }
        table[level][sym->GetSymbolValue()] = Token(PROC, level, addr[level]++, 0);
        Getsym();
        if(sym->GetSymbolTag() != SYM_SEMICOLON) {
            // expect SEMICOLON; here
            Halt(1);
        }

        level++;
        Block();
        level--;

        Getsym();
        if(sym->GetSymbolTag() != SYM_PROC) {
            lex->PrependSymbol(sym);
            break;
        }
    }
}

void GrammarAnalyzer::Stmts() {
    Getsym();
    lex->PrependSymbol(sym);
    switch(sym->GetSymbolTag()) {
    case SYM_IDENT: Assign();    break;  // 赋值语句
    case SYM_IF:    Condition(); break; // 条件语句
    case SYM_WHILE: Whiledo();   break; // 当型循环语句
    case SYM_CALL:  Call();      break; // 过程调用语句
    case SYM_READ:  Read();      break; // 读语句
    case SYM_WRITE: Write();     break; // 写语句
    case SYM_BEGIN: Compound();  break; // 复合语句
    default: break;
    }
}

void GrammarAnalyzer::Assign() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_IDENT) {
        // expected IDENT here
        Halt(1);
    }
    Token* token = FindToken(sym->GetSymbolValue());
    if(!token || token->kind != VAR) {
        // cannot find token or not a variable
        Halt(1);
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_BECOMES) {
        // expect BECOME:= here
        Halt(1);
    }
    Expr();
}

void GrammarAnalyzer::Compound() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_BEGIN) {
        // expected BEGIN here
        Halt(1);
    }
    while(true) {
        Stmts();
        Getsym();
        switch(sym->GetSymbolTag()) {
        case SYM_SEMICOLON: break;
        case SYM_END: return;
        default: Halt(1);    // epected SEMICOLON or END here
        }
    }
}

void GrammarAnalyzer::Cond() {
    Getsym();
    if(sym->GetSymbolTag() == SYM_ODD) {
        Expr();
        return;
    }
    lex->PrependSymbol(sym);
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
        Halt(1);    // expected =,#,<,<=,>,>= here
    }
    Expr();
}

void GrammarAnalyzer::Expr() {
    while(true) {
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_PLUS:
        case SYM_MINUS:
            break;
        default:
            lex->PrependSymbol(sym);
            return;
        }
        Term();
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
            lex->PrependSymbol(sym);
            return;
        }
        Factor();
    }
}

void GrammarAnalyzer::Factor() {
    Getsym();
    switch (sym->GetSymbolTag())
    {
    case SYM_IDENT:
        break;
    case SYM_NUMBER:
        break;
    case SYM_LPAREN:
        Expr();
        Getsym();
        if(sym->GetSymbolTag() != SYM_RPAREN) {
            // expected RPAREN) here
            Halt(1);
        }
        break;
    default:
        Halt(1);    // nothing matched
    }
}

void GrammarAnalyzer::Condition() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_IF) {
        // expected IF here
        Halt(1);
    }
    Cond();
    Getsym();
    if(sym->GetSymbolTag() != SYM_THEN) {
        // expected THEN here
        Halt(1);
    }
    Stmts();
}

void GrammarAnalyzer::Whiledo() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_WHILE) {
        // expected WHILE here
        Halt(1);
    }
    Cond();
    Getsym();
    if(sym->GetSymbolTag() != SYM_DO) {
        // expect DO here;
        Halt(1);
    }
    Stmts();
}

void GrammarAnalyzer::Call() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_CALL) {
        // expected CALL here
        Halt(1);
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_IDENT) {
        // expected IDENT here
        Halt(1);
    }
    Token* token = FindToken(sym->GetSymbolValue());
    if(!token || token->kind != PROC) {
        // cannot find token or not a proc
        Halt(1);
    }
}

void GrammarAnalyzer::Read() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_READ) {
        // expected READ here
        Halt(1);
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_LPAREN) {
        // expected LPAREN( here
        Halt(1);
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expected IDENT here
            Halt(1);
        }
        Token* token = FindToken(sym->GetSymbolValue());
        if(!token || token->kind == PROC) {
            // expected var or const here
            Halt(1);
        }
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_COMMA: break;
        case SYM_RPAREN: return;
        default:
            Halt(1);    // expected COMMA, or RPAREN) here
        }
    }
}

void GrammarAnalyzer::Write() {
    Getsym();
    if(sym->GetSymbolTag() != SYM_WRITE) {
        // expected READ here
        Halt(1);
    }
    Getsym();
    if(sym->GetSymbolTag() != SYM_LPAREN) {
        // expected LPAREN( here
        Halt(1);
    }
    while(true) {
        Getsym();
        if(sym->GetSymbolTag() != SYM_IDENT) {
            // expected IDENT here
            Halt(1);
        }
        Token* token = FindToken(sym->GetSymbolValue());
        if(!token || token->kind == PROC) {
            // expected var or const here
            Halt(1);
        }
        Getsym();
        switch (sym->GetSymbolTag())
        {
        case SYM_COMMA: break;
        case SYM_RPAREN: return;
        default:
            Halt(1);    // expected COMMA, or RPAREN) here
        }
    }
}