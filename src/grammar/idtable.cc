#include "idtable.h"

IdTable::IdTable(): preTable(nullptr) {}

bool IdTable::SetIdentifier(std::string name, Identifier* ident) {
    if(table.count(name))   return false;
    table[name] = ident;
    return true;
}

Identifier* IdTable::GetIdentifier(std::string name) {
    if(table.count(name))   return table[name];
    return nullptr;
}

bool IdTable::SetPreTable(IdTable* pre) {
    this->preTable = pre;
    return true;
}

IdTable* IdTable::GetPreTable() { return this->preTable; }

std::map<std::string, Identifier*>* IdTable::GetIdentifierTable() { return &this->table; }

void IdTable::ListTable() {
    IdTable::print(this, 0);
}

void IdTable::print(IdTable* table, int dep) {
    if(table == nullptr)    return;
    for(auto it:table->table) {
        for(int i = 0; i < dep; ++i)    std::cout << "  ";
        std::cout << it.first << ' ';
        it.second->print();
        if(it.second->kind == PROC) {
            IdTable::print(it.second->link, dep + 1);
        }
    }
}