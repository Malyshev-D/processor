#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#define PROCESSOR
#define CMDARRAY

#include "stack.h" // stack.cpp
#include "core.h"
#include "commands.h" // commands.cpp
#include "../enum.h"
#include "readswitch.h" // readswitch.cpp

const char code_file[] = "../square.txt";

int main()
{
    processor_t vm = {};
    StackInit(&(vm.stk), 8);
    StackInit(&(vm.func_stk), 2);
    ReadByteCode(code_file, &vm);

    CmdSwitch(&vm, cmd_array);
    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%d ", vm.regs[i]);
    // }
    return 0;
}
