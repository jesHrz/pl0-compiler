#ifndef __IDTABLE_H_
#define __IDTABLE_H_

#include <iomanip>
#include <map>
#include <string>
#include "symbol.h"

enum KIND {
    NIL,
    CONST,
    VAR,
    PROC,
};

class IdTable;

class Identifier {
public:
    Identifier(enum KIND kind=NIL, int level=-1, int addr=-1, num_t value=0, IdTable* link=nullptr): kind(kind), level(level), addr(addr), value(value), link(link) {}
    Identifier(const Identifier& t): kind(t.kind), level(t.level), addr(t.addr), value(t.value), link(t.link) {}
    Identifier& operator=(const Identifier& t) {
        if(this == &t)  return *this;
        kind = t.kind;
        level = t.level;
        addr = t.addr;
        value = t.value;
        link = t.link;
        return *this;
    };

    KIND kind;
    int level;
    int addr;
    num_t value;

    IdTable* link;

    void print() const {
        std::cout << std::setw(3) << kind << ' ' << level << ' ' << addr << ' ' << value << ' ' << link << std::endl;
    }
};

class IdTable {
public:
    IdTable();
    bool SetIdentifier(std::string name, Identifier* ident);
    Identifier* GetIdentifier(std::string name);
    bool SetPreTable(IdTable* pre);
    IdTable* GetPreTable();
    std::map<std::string, Identifier*>* GetIdentifierTable();

    void ListTable();
private:
    IdTable* preTable;
    std::map<std::string, Identifier*> table;

    static void print(IdTable*, int dep);

private:
    IdTable(const IdTable&) = delete;
    IdTable& operator=(const IdTable&) const = delete;
};

#endif // __IDTABLE_H_