#ifndef __VM_H_
#define __VM_H_

#include "grammar/pcode.h"


#define STACK_SIZE 1024
#define STACK_TYPE unsigned int

class VirtualMachine {
public:
    explicit VirtualMachine(CodeTable *pcodes);

    ~VirtualMachine();

    void run();

private:
    STACK_TYPE *stack;
    size_t sp, bp, pc;
    CodeTable *pcodes;

    void exception(pcode *ir = nullptr);

    void CheckStackOverflow() const;
};

#endif /* __VM_H_ */