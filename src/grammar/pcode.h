#ifndef __CODE_H_
#define __CODE_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

enum {
    LIT, LOD, STO, CAL, INT, JMP, JPC, OPR
};

static std::string F_NAME[] = {
        "LIT", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "OPR"
};

enum {
    OPR_RET,    // return
    OPR_NEG,    // minus
    OPR_ADD,    // +
    OPR_SUB,    // -
    OPR_MUL,    // *
    OPR_DIV,    // /
    OPR_EQL,    // =
    OPR_NEQ,    // #
    OPR_LSS,    // <
    OPR_LEQ,    // <=
    OPR_GTR,    // >
    OPR_GEQ,    // >=
    OPR_ODD,    // odd
    OPR_RD,     // read
    OPR_WR      // write
};

static std::string OPR_NAME[] = {
        "OPR_RET", "OPR_NEG",
        "OPR_ADD", "OPR_SUB", "OPR_MUL", "OPR_DIV",
        "OPR_EQL", "OPR_NEQ", "OPR_LSS", "OPR_LEQ", "OPR_GTR", "OPR_GEQ", "OPR_ODD",
        "OPR_RD", "OPR_WR"
};

class pcode {
public:
    pcode(int f, int l, int a) : f(f), l(l), a(a) {};

    pcode &operator=(const pcode &t) {
        if (this == &t) return *this;
        this->f = t.f;
        this->l = t.l;
        this->a = t.a;
        return *this;
    };

    int GetF() const { return f; }

    int GetL() const { return l; }

    int GetA() const { return a; }

    void BackPatch(int addr) { this->a = addr; };
private:
    int f, l, a;
};

class CodeTable
        : public std::vector<pcode *> {
public:
    ~CodeTable() {
        for (int i = 0; i < size(); ++i)
            delete this->at(i);
    };

    pcode *Gen(int f, int l, int a) {
        auto *newCode = new pcode(f, l, a);
        push_back(newCode);
        return newCode;
    };

    pcode *Gen(pcode *code) {
        push_back(code);
        return code;
    }

    pcode *at(int k) const {
        return this->operator[](k);
    };

    void ListCodes(std::ostream &out = std::cout) const {
        for (int i = 0; i < size(); ++i) {
            pcode *tmp = this->at(i);
            out << std::setw(5) << i << "\t("
                << F_NAME[tmp->GetF()] << ", "
                << tmp->GetL() << ", "
                << tmp->GetA();
            if (tmp->GetF() == OPR) {
                out << "[" << OPR_NAME[tmp->GetA()] << "]";
            }
            out << ")" << std::endl;
        }
    };
};

#endif /* __CODE_H_ */