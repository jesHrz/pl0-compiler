#ifndef __VM_H_
#define __VM_H_

#include "grammar/pcode.h"


#define STACK_SIZE 1024

class VirtualMachine {
public:
    explicit VirtualMachine(CodeTable *pcodes);

    ~VirtualMachine();

    void run();

private:
    int *stack;
    int sp, bp, pc;
    CodeTable *pcodes;

    void exception(pcode *ir = nullptr);
};

#endif /* __VM_H_ */