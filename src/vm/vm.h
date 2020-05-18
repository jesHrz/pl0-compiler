#ifndef __VM_H_
#define __VM_H_

#include "grammar/pcode.h"

class VirtualMachine {
public:
    VirtualMachine(CodeTable *pcodes);

    ~VirtualMachine();

    void run();

private:
    CodeTable *pcodes;
    int *stack;
    size_t sp, bp, pc;

    void exception(int operation);
};

#endif /* __VM_H_ */