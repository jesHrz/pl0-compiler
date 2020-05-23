#include <queue>
#include "grammar/idtable.h"

IdTable::IdTable() : preTable(nullptr) {}

bool IdTable::SetIdentifier(const std::string &name, Identifier *ident) {
    if (table.count(name)) return false;
    table[name] = ident;
    return true;
}

Identifier *IdTable::GetIdentifier(const std::string &name) {
    if (table.count(name)) return table[name];
    return nullptr;
}

bool IdTable::SetPreTable(IdTable *pre) {
    this->preTable = pre;
    return true;
}

IdTable *IdTable::GetPreTable() { return this->preTable; }

std::map<std::string, Identifier *> *IdTable::GetIdentifierTable() { return &this->table; }

void IdTable::ListIdTable(std::ostream &out) {
    std::queue<IdTable *> q;
    q.push(this);
    while (!q.empty()) {
        IdTable *pTable = q.front();
        q.pop();
        out << "TABLE at " << pTable << std::endl;
        out << std::setw(10) << "NAME"
            << std::setw(12) << "KIND"
            << std::setw(12) << "LEVEL"
            << std::setw(12) << "ADDR"
            << std::setw(12) << "VALUE"
            << std::setw(20) << "LINK"
            << std::endl;
        for (const auto &it:pTable->table) {
            out << std::right << std::setw(10) << it.first;
            it.second->Print(out);
            if (it.second->kind == PROC) {
                q.push(it.second->link);
            }
        }
        std::cout << std::endl;
    }
}