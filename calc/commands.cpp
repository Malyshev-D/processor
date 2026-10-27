#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>

#include "stack.h"
#include "core.h"
#include "commands.h"


void Push(processor_t* vm)
{
    StackPush(&(vm->stk), 1000 * vm->code[vm->ip]);
}

void Add(processor_t* vm)
{
    int value1, value2;
    StackPop(&(vm->stk), &value1);
    StackPop(&(vm->stk), &value2);
    StackPush(&(vm->stk), value1 + value2);
}

void Sub(processor_t* vm)
{
    int value1, value2;
    StackPop(&(vm->stk), &value1);
    StackPop(&(vm->stk), &value2);
    StackPush(&(vm->stk), value2 - value1);
}

void Mul(processor_t* vm)
{
    int value1, value2;
    StackPop(&(vm->stk), &value1);
    StackPop(&(vm->stk), &value2);
    StackPush(&(vm->stk), value1 * value2);
}

void Div(processor_t* vm)
{
    int value1, value2;
    StackPop(&(vm->stk), &value1);
    StackPop(&(vm->stk), &value2);
    StackPush(&(vm->stk), value2 / value1);
}

void Away(processor_t* vm)
{
    int value;
    StackPop(&(vm->stk), &value);
    printf("%d\n", value / 1000);
}

void Hlt(processor_t* vm)
{
    StackDestroy(&(vm->stk));
}

void Pop_reg(processor_t* vm)
{
    StackPop(&(vm->stk), vm->regs + vm->code[vm->ip] - 1);
}

void Push_reg(processor_t* vm)
{
    StackPush(&(vm->stk), vm->regs[vm->code[vm->ip] - 1]);
}

void Jump(processor_t* vm)
{
    vm->ip = vm->code[vm->ip] - 1;
}

void JumpIfAbove(processor_t* vm)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a > b)
        Jump(vm);

}

void JumpIfAboveEqual(processor_t* vm)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a >= b)
        Jump(vm);

}

void JumpIfBelow(processor_t* vm)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a < b)
        Jump(vm);

}

void JumpIfBelowEqual(processor_t* vm)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a <= b)
        Jump(vm);

}

void JumpIfEqual(processor_t* vm)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a == b)
        Jump(vm);

}

void JumpIfNotEqual(processor_t* vm)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a != b)
        Jump(vm);

}

void Call(processor_t* vm)
{
    StackPush(&(vm->func_stk), vm->ip);
    Jump(vm);
}

void Ret(processor_t* vm)
{
    int label = 0;
    StackPop(&(vm->func_stk), &label);
    Jump(vm);
}

void Dump(processor_t* vm)
{
    int cmd = 0;
    for (cmd = 0; cmd < vm->last_idx; cmd++)
    {
        if (cmd == vm->ip)
        {
            printf(ANSI_COLOR_YELLOW "[%2d] " ANSI_COLOR_RESET, vm->code[cmd]);
        }
        else
            printf("%2d ", vm->code[cmd]);
    }
    putchar('\n');
    for (cmd = 0; cmd < vm->last_idx; cmd++)
    {
        if (cmd == vm->ip)
        {
            printf(ANSI_COLOR_YELLOW "[%2d] " ANSI_COLOR_RESET, cmd);
        }
        else
            printf("%2d ", cmd);
    }
    putchar('\n');
}

void Scan(processor_t* vm)
{
    int value = 0;
    scanf("%d", &value);
    StackPush(&(vm->stk), 1000 * value);
}

void Sqrt(processor_t* vm)
{
    int value = 0;
    StackPop(&(vm->stk), &value);
    value = (int) sqrt(value);
    StackPush(&(vm->stk), value);
}
