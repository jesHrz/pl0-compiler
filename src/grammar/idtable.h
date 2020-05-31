#ifndef __IDTABLE_H_
#define __IDTABLE_H_

#include <iomanip>
#include <map>
#include <string>
#include "symbol/symbol.h"

enum KIND {
    NIL,
    CONST,
    VAR,
    PROC,
};

static std::string KIND_NAME[] = {
        "NIL", "CONST", "VAR", "PROC"
};

class IdTable;

class Identifier {
public:
    explicit Identifier(enum KIND kind = NIL, int level = 0, int addr = 0, num_t value = 0, IdTable *link = nullptr)
            : kind(kind), level(level), addr(addr), value(value), link(link) {}

    Identifier &operator=(const Identifier &t) {
        if (this == &t) return *this;
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

    IdTable *link;

    void Print(std::ostream &out = std::cout) const {
        out << std::setw(12) << KIND_NAME[kind]
            << std::setw(12) << level
            << std::setw(12) << addr
            << std::setw(12) << value
            << std::setw(20) << link
            << std::endl;
    }
};

class IdTable {
public:
    IdTable();

    bool SetIdentifier(const std::string &name, Identifier *ident);

    Identifier *GetIdentifier(const std::string &name);

    bool SetPreTable(IdTable *pre);

    IdTable *GetPreTable();

    std::map<std::string, Identifier *> *GetIdentifierTable();

    void ListIdTable(std::ostream &out = std::cout);

private:
    IdTable *preTable;
    std::map<std::string, Identifier *> table;

public:
    IdTable(const IdTable &) = delete;

    IdTable &operator=(const IdTable &) const = delete;
};

#endif // __IDTABLE_H_