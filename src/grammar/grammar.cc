#include "grammar/grammar.h"
#include <vector>
#include <queue>

#ifdef PRINT_GRAMMAR_TREE
int _depth;
#define _recur(...) do {\
                        for(int i = 0; i < _depth; ++i)  printf("| ");\
                        printf("|-- ");\
                        printf(__VA_ARGS__);\
                        printf("\n");\
                        _depth++; \
                    } while(0)

#define _back(k) { _depth -= (k); }

#else
#define _recur(...)

#define _back(k)

#endif

extern void Error(const char *);

extern void Error(Symbol *, const char *);

extern void Error(Symbol *, std::vector<sym_t>);

GrammarAnalyzer::GrammarAnalyzer(LexAnalyzer *lex, CodeTable *pcodes) : lex(lex), pcodes(pcodes) {
    addr = new int[RECURSION_DEPTH];
    for (int i = 0; i < RECURSION_DEPTH; ++i) addr[i] = 0;
    curTable = mainTable = nullptr;
    sym = nullptr;
    level = 0;
}

GrammarAnalyzer::~GrammarAnalyzer() {
    std::queue<IdTable *> q;
    q.push(mainTable);
    while (!q.empty()) {
        IdTable *top = q.front();
        q.pop();
        if (!top) continue;
        auto table = top->GetIdentifierTable();
        for (auto &it : *table) {
            if (it.second->link) q.push(it.second->link);
        }
        delete top;
    }
    delete[] addr;
}

void GrammarAnalyzer::Getsym() {
    if (sym != nullptr) {
        delete sym;
        sym = nullptr;
    }
    sym = lex->GetSymbol();
    if (sym == nullptr) {
        // end of file
        Error("End of file, abort.");
    }
}

void GrammarAnalyzer::Retract() {
    lex->PrependSymbol(sym);
    sym = nullptr;
}

void GrammarAnalyzer::Program() {
    _recur(__FUNCTION__);
    curTable = mainTable = new IdTable;
    Block();
    Getsym();
    if (sym->GetSymbolTag() != SYM_PERIOD) {
        // expect period. here
        Error(sym, {SYM_PERIOD});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        pcodes->Gen(OPR, 0, OPR_RET);
    }
    _back(1);
}

void GrammarAnalyzer::Block() {
    _recur(__FUNCTION__);
    if (level >= RECURSION_DEPTH) {
        // recursion overflow
        Error("Too many recursions, abort.");
    }
    addr[level] += 3;
    pcodes->Gen(INT, 0, 3);

    sym_t tmp[] = {SYM_CONST, SYM_VAR, SYM_PROC};
    Getsym();
    for (int i = 0; i < 3; ++i) {
        if (sym->GetSymbolTag() == tmp[i]) {
            Retract();
            switch (i) {
            case 0:
                ConstDecl();
                break;
            case 1:
                VarDecl();
                break;
            case 2:
                ProcDecl();
                break;
            default:
                break;
            }
            Getsym();
        }
    }
    Retract();
    Stmts();
    _back(1);
}

void GrammarAnalyzer::ConstDecl() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_CONST) {
        // expected CONST here
        Error(sym, {SYM_CONST});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    int dep = 0;
    while (true) {
        dep++;
        Getsym();
        if (sym->GetSymbolTag() != SYM_IDENT) {
            // expect ident here
            Error(sym, {SYM_IDENT});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }

        std::string constName = sym->GetSymbolValue();
        Getsym();
        if (sym->GetSymbolTag() != SYM_EQL) {
            // expect EQL= here
            Error(sym, {SYM_EQL});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }
        Getsym();
        if (sym->GetSymbolTag() != SYM_NUMBER) {
            // expect NUMBER here
            Error(sym, {SYM_NUMBER});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }
        auto *ident = new Identifier(CONST, 0, 0, sym->GetNumber());
        if (!curTable->SetIdentifier(constName, ident)) {
            delete ident;
            Error(sym, "redefined identifier");
        }

        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_COMMA:
            _recur("%sRep", __FUNCTION__);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            break;
        case SYM_SEMICOLON:
            _back(dep - 1);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(2);
            return;
        default:
            Error(sym, {SYM_COMMA, SYM_SEMICOLON});   // expect COMMA, or SEMICOLON; here
        }
    }
}

void GrammarAnalyzer::VarDecl() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_VAR) {
        // expected VAR here
        Error(sym, {SYM_VAR});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    int dep = 0;
    while (true) {
        dep++;
        Getsym();
        if (sym->GetSymbolTag() != SYM_IDENT) {
            // expect IDENT here
            Error(sym, {SYM_IDENT});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }

        std::string varName = sym->GetSymbolValue();
        auto *ident = new Identifier(VAR, level, addr[level]++, 0);
        if (!curTable->SetIdentifier(varName, ident)) {
            Error(sym, "redefined identifier");
        }

        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_COMMA:
            _recur("%sRep", __FUNCTION__);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            break;
        case SYM_SEMICOLON:
            _back(dep - 1);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(2);
            pcodes->Gen(INT, 0, dep);
            return;
        default:
            Error(sym, {SYM_COMMA, SYM_SEMICOLON});   // expect COMMA, or SEMICOLON; here
        }
    }
}

void GrammarAnalyzer::ProcDecl() {
    _recur(__FUNCTION__);
    int dep = 0;
    while (true) {
        dep++;
        Getsym();
        if (sym->GetSymbolTag() != SYM_PROC) {
            // expected PROCEDURE here
            Error(sym, {SYM_PROC});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }
        Getsym();
        if (sym->GetSymbolTag() != SYM_IDENT) {
            // expect IDENT here
            Error(sym, {SYM_IDENT});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }

        std::string procName = sym->GetSymbolValue();
        auto *ident = new Identifier(PROC, level++);
        if (!curTable->SetIdentifier(procName, ident)) {
            delete ident;
            Error(sym, "redefined identifier");
        }

        Getsym();
        if (sym->GetSymbolTag() != SYM_SEMICOLON) {
            // expect SEMICOLON; here
            Error(sym, {SYM_SEMICOLON});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }

        pcode *tmp = pcodes->Gen(JMP, 0, 0);
        ident->link = new IdTable;
        ident->link->SetPreTable(curTable);
        ident->addr = pcodes->size();
        curTable = ident->link;
        Block();
        addr[--level] = 0;
        curTable = curTable->GetPreTable();
        pcodes->Gen(OPR, 0, OPR_RET);
        tmp->BackPatch(pcodes->size());

        Getsym();
        if (sym->GetSymbolTag() != SYM_SEMICOLON) {
            // expect SEMICOLON; here
            Error(sym, {SYM_SEMICOLON});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }
        Getsym();
        if (sym->GetSymbolTag() != SYM_PROC) {
            _back(dep - 1);
            _recur("SYM_NUL");
            _back(1);
            Retract();
            break;
        }
        Retract();
    }
    _back(1);
}

void GrammarAnalyzer::Stmts() {
    _recur(__FUNCTION__);
    Getsym();
    switch (sym->GetSymbolTag()) {
    case SYM_IDENT:
        Retract();
        Assign();
        break;  // 赋值语句
    case SYM_IF:
        Retract();
        Condition();
        break; // 条件语句
    case SYM_WHILE:
        Retract();
        Whiledo();
        break; // 当型循环语句
    case SYM_CALL:
        Retract();
        Call();
        break; // 过程调用语句
    case SYM_READ:
        Retract();
        Read();
        break; // 读语句
    case SYM_WRITE:
        Retract();
        Write();
        break; // 写语句
    case SYM_BEGIN:
        Retract();
        Compound();
        break; // 复合语句
    default:
        _recur("SYM_NUL");
        _back(1);
        Retract();
        break;
    }
    _back(1);
}

void GrammarAnalyzer::Assign() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_IDENT) {
        // expected IDENT here
        Error(sym, {SYM_IDENT});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    Identifier *ident = FindIdentifier(sym->GetSymbolValue());
    if (!ident) {
        Error(sym, "undefined identifier");
    }
    if (ident->kind != VAR) {
        Error(sym, "identifier is not a var");
    }
    Getsym();
    if (sym->GetSymbolTag() != SYM_BECOMES) {
        // expect BECOME:= here
        Error(sym, {SYM_BECOMES});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    Expr();
    pcodes->Gen(STO, abs(ident->level - level), ident->addr);
    _back(1);
}

void GrammarAnalyzer::Compound() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_BEGIN) {
        // expected BEGIN here
        Error(sym, {SYM_BEGIN});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    int dep = 0;
    while (true) {
        dep++;
        Stmts();
        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_SEMICOLON:
            _recur("%sRep", __FUNCTION__);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            break;
        case SYM_END:
            _back(dep - 1);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(2);
            return;
        default:
            Error(sym, {SYM_SEMICOLON, SYM_END});    // epected SEMICOLON or END here
        }
    }
}

void GrammarAnalyzer::Cond() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() == SYM_ODD) {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
        Expr();
        pcodes->Gen(OPR, 0, OPR_ODD);
        _back(1);
        return;
    }
    Retract();
    Expr();
    Getsym();
    sym_t tag = sym->GetSymbolTag();
    switch (tag) {
    case SYM_EQL:
    case SYM_NEQ:
    case SYM_LSS:
    case SYM_LEQ:
    case SYM_GTR:
    case SYM_GEQ:
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
        break;
    default:
        Error(sym, {SYM_EQL, SYM_NEQ, SYM_LSS, SYM_LEQ, SYM_GTR, SYM_GEQ}); // expected =,#,<,<=,>,>= here
    }
    Expr();
    switch (tag) {
    case SYM_EQL:
        pcodes->Gen(OPR, 0, OPR_EQL);
        break;
    case SYM_NEQ:
        pcodes->Gen(OPR, 0, OPR_NEQ);
        break;
    case SYM_LSS:
        pcodes->Gen(OPR, 0, OPR_LSS);
        break;
    case SYM_LEQ:
        pcodes->Gen(OPR, 0, OPR_LEQ);
        break;
    case SYM_GTR:
        pcodes->Gen(OPR, 0, OPR_GTR);
        break;
    case SYM_GEQ:
        pcodes->Gen(OPR, 0, OPR_GEQ);
        break;
    default:
        break;
    }
    _back(1);
}

void GrammarAnalyzer::Expr() {
    _recur(__FUNCTION__);
    pcode *lastCode = nullptr;
    Getsym();
    if (sym->GetSymbolTag() != SYM_PLUS && sym->GetSymbolTag() != SYM_MINUS) {
        Retract();
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
        if (sym->GetSymbolTag() == SYM_MINUS) lastCode = new pcode(OPR, 0, OPR_NEG);
    }
    int dep = 0;
    Term();
    if (lastCode) pcodes->Gen(lastCode);
    while (true) {
        dep++;
        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_PLUS:
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            lastCode = new pcode(OPR, 0, OPR_ADD);
            break;
        case SYM_MINUS:
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            lastCode = new pcode(OPR, 0, OPR_SUB);
            break;
        default:
            _recur("SYM_NUL");
            _back(dep + 1);
            Retract();
            return;
        }
        Term();
        pcodes->Gen(lastCode);
        _recur("%sRep", __FUNCTION__);
    }
}

void GrammarAnalyzer::Term() {
    _recur(__FUNCTION__);
    pcode *lastCode;
    Factor();
    int dep = 0;
    while (true) {
        dep++;
        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_TIMES:
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            lastCode = new pcode(OPR, 0, OPR_MUL);
            break;
        case SYM_SLASH:
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            lastCode = new pcode(OPR, 0, OPR_DIV);
            break;
        default:
            _recur("SYM_NUL");
            _back(dep + 1);
            Retract();
            return;
        }
        Factor();
        pcodes->Gen(lastCode);
        _recur("%sRep", __FUNCTION__);
    }
}

void GrammarAnalyzer::Factor() {
    _recur(__FUNCTION__);
    Identifier *ident;
    Getsym();
    switch (sym->GetSymbolTag()) {
    case SYM_IDENT:
        ident = FindIdentifier(sym->GetSymbolValue());
        if (!ident) {
            Error(sym, "undefined identifier");
        }
        switch (ident->kind) {
        case CONST:
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            pcodes->Gen(LIT, 0, ident->value);
            break;
        case VAR:
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            pcodes->Gen(LOD, abs(ident->level - level), ident->addr);
            break;
        default:
            Error(sym, "identifier is not a var or const");
        }
        break;
    case SYM_NUMBER:
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
        pcodes->Gen(LIT, 0, sym->GetNumber());
        break;
    case SYM_LPAREN:
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
        Expr();
        Getsym();
        if (sym->GetSymbolTag() != SYM_RPAREN) {
            // expected RPAREN) here
            Error(sym, {SYM_RPAREN});
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
        }
        break;
    default:
        Error(sym, {SYM_IDENT, SYM_NUMBER, SYM_LPAREN});    // nothing matched
    }
    _back(1);
}

void GrammarAnalyzer::Condition() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_IF) {
        // expected IF here
        Error(sym, {SYM_IF});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    Cond();
    Getsym();
    if (sym->GetSymbolTag() != SYM_THEN) {
        // expected THEN here
        Error(sym, {SYM_THEN});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    pcode *tmp = pcodes->Gen(JPC, 0, 0);
    Stmts();
    tmp->BackPatch(pcodes->size());
    _back(1);
}

void GrammarAnalyzer::Whiledo() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_WHILE) {
        // expected WHILE here
        Error(sym, {SYM_WHILE});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    int BEGIN = pcodes->size();
    Cond();
    Getsym();
    if (sym->GetSymbolTag() != SYM_DO) {
        // expect DO here;
        Error(sym, {SYM_DO});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    pcode *tmp = pcodes->Gen(JPC, 0, 0);
    Stmts();
    pcodes->Gen(JMP, 0, BEGIN);
    tmp->BackPatch(pcodes->size());
    _back(1);
}

void GrammarAnalyzer::Call() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_CALL) {
        // expected CALL here
        Error(sym, {SYM_CALL});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    Getsym();
    if (sym->GetSymbolTag() != SYM_IDENT) {
        // expected IDENT here
        Error(sym, {SYM_IDENT});
    }
    Identifier *ident = FindIdentifier(sym->GetSymbolValue());
    if (!ident) {
        Error(sym, "undefined indentifier");
    }
    if (ident->kind != PROC) {
        Error(sym, "identifier is not a procedure");
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
        pcodes->Gen(CAL, abs(ident->level - level), ident->addr);
    }
    _back(1);
}

void GrammarAnalyzer::Read() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_READ) {
        // expected READ here
        Error(sym, {SYM_READ});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    Getsym();
    if (sym->GetSymbolTag() != SYM_LPAREN) {
        // expected LPAREN( here
        Error(sym, {SYM_LPAREN});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    int dep = 0;
    while (true) {
        dep++;
        Getsym();
        if (sym->GetSymbolTag() != SYM_IDENT) {
            // expected IDENT here
            Error(sym, {SYM_IDENT});
        }
        Identifier *ident = FindIdentifier(sym->GetSymbolValue());
        if (!ident) {
            Error(sym, "undefined identifer");
        }
        if (ident->kind != VAR && ident->kind != CONST) {
            Error(sym, "identifier is not a var or const");
        } else {
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            pcodes->Gen(OPR, 0, OPR_RD);
            pcodes->Gen(STO, abs(ident->level - level), ident->addr);
        }
        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_COMMA:
            _recur("%sRep", __FUNCTION__);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            break;
        case SYM_RPAREN:
            _back(dep - 1);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(2);
            return;
        default:
            Error(sym, {SYM_COMMA, SYM_RPAREN});    // expected COMMA, or RPAREN) here
        }
    }
}

void GrammarAnalyzer::Write() {
    _recur(__FUNCTION__);
    Getsym();
    if (sym->GetSymbolTag() != SYM_WRITE) {
        // expected READ here
        Error(sym, {SYM_WRITE});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    Getsym();
    if (sym->GetSymbolTag() != SYM_LPAREN) {
        // expected LPAREN( here
        Error(sym, {SYM_LPAREN});
    } else {
        _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
        _back(1);
    }
    int dep = 0;
    while (true) {
        dep++;
        Expr();
        pcodes->Gen(OPR, 0, OPR_WR);
        Getsym();
        switch (sym->GetSymbolTag()) {
        case SYM_COMMA:
            _recur("%sRep", __FUNCTION__);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(1);
            break;
        case SYM_RPAREN:
            _back(dep - 1);
            _recur("Symbol [%s]", sym->GetSymbolValue().c_str());
            _back(2);
            return;
        default:
            Error(sym, {SYM_COMMA, SYM_RPAREN});    // expected COMMA, or RPAREN) here
        }
    }
}

void GrammarAnalyzer::ListIdTable() const { mainTable->ListIdTable(); }
