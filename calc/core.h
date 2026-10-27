#ifndef INTELCORE
#define INTELCORE

struct processor_t {
    stack_t stk;
    int     code[1000];
    int     regs[4];
    int     ip;
    stack_t func_stk;
    int     last_idx;
};

#endif
