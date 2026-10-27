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

const char code_file[] = "../code.txt";

int main()
{
    processor_t vm = {};
    StackInit(&(vm.stk), 8);
    StackInit(&(vm.func_stk), 2);
    ReadByteCode(code_file, &vm);

    CmdSwitch(&vm, cmd_array);

    return 0;
}
