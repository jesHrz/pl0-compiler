#include "vm/vm.h"

#include "grammar/pcode.h"

VirtualMachine::VirtualMachine(CodeTable *pcodes) : sp(0), bp(0), pc(0) {
    this->pcodes = pcodes;
    this->stack = new STACK_TYPE[STACK_SIZE];
}

VirtualMachine::~VirtualMachine() {
    delete[] this->stack;
}

void VirtualMachine::run() {
    auto base = [&](int l) -> int {
        size_t b = bp;
        for (; l > 0; --l) b = stack[b];
        return b;
    };
    // initialize
    // bottom->top: static link, dynamic link, return address
    sp = pc = 0;
    bp = 1;
    stack[1] = stack[2] = 0;
    stack[3] = STACK_SIZE;
    pcode *ir;
    while (pc < STACK_SIZE) {
        ir = pcodes->at(pc++);
        switch (ir->GetF()) {
        case LIT:
            stack[++sp] = ir->GetA();
            break;
        case LOD:
            stack[++sp] = stack[base(ir->GetL()) + ir->GetA()];
            break;
        case STO:
            stack[base(ir->GetL()) + ir->GetA()] = stack[sp--];
            break;
        case CAL:
            stack[sp + 1] = base(ir->GetL());
            stack[sp + 2] = bp;
            stack[sp + 3] = pc;
            bp = sp + 1;
            pc = ir->GetA();
            break;
        case INT:
            sp += ir->GetA();
            break;
        case JMP:
            pc = ir->GetA();
            break;
        case JPC:
            if (!stack[sp--]) pc = ir->GetA();
            break;
        case OPR:
            switch (ir->GetA()) {
            case OPR_RET:
                sp = bp - 1;
                bp = stack[sp + 2];
                pc = stack[sp + 3];
                break;
            case OPR_NEG:
                stack[sp] = -stack[sp];
                break;
            case OPR_ADD:
                stack[sp - 1] += stack[sp];
                sp--;
                break;
            case OPR_SUB:
                stack[sp - 1] -= stack[sp];
                sp--;
                break;
            case OPR_MUL:
                stack[sp - 1] *= stack[sp];
                sp--;
                break;
            case OPR_DIV:
                stack[sp - 1] /= stack[sp];
                sp--;
                break;
            case OPR_EQL:
                stack[sp - 1] = (stack[sp - 1] == stack[sp]);
                sp--;
                break;
            case OPR_NEQ:
                stack[sp - 1] = (stack[sp - 1] != stack[sp]);
                sp--;
                break;
            case OPR_LSS:
                stack[sp - 1] = (stack[sp - 1] < stack[sp]);
                sp--;
                break;
            case OPR_LEQ:
                stack[sp - 1] = (stack[sp - 1] <= stack[sp]);
                sp--;
                break;
            case OPR_GTR:
                stack[sp - 1] = (stack[sp - 1] > stack[sp]);
                sp--;
                break;
            case OPR_GEQ:
                stack[sp - 1] = (stack[sp - 1] >= stack[sp]);
                sp--;
                break;
            case OPR_ODD:
                stack[sp] = !stack[sp];
                break;
            case OPR_RD:
            case OPR_WR:
                exception(ir);
                break;
            default:
                exception();
            }
            break;
        default:
            exception();
        }
        CheckStackOverflow();
    }
}

extern void Halt(int);

void VirtualMachine::exception(pcode *ir) {
    if (!ir) {
        perror("Unexpected object code!");
        Halt(-1);
    }
    switch (ir->GetF()) {
    case OPR:
        switch (ir->GetA()) {
        case OPR_RD:
            std::cin >> this->stack[++sp];
            break;
        case OPR_WR:
            std::cout << this->stack[sp--] << std::endl;
            break;
        default:
            break;
        }
    default:
        break;
    }
}

void VirtualMachine::CheckStackOverflow() const {
    if (sp >= STACK_SIZE) {
        perror("RuntimeError: stack overflow!");
        Halt(-1);
    }
}
