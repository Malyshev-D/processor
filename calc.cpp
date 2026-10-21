#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include "stack.h"
#include "enum.h"

struct processor_t {
    stack_t stk;
    int code[1000];
    int regs[4];
};

void Push     (processor_t* vm, int value);
void Add      (processor_t* vm);
void Sub      (processor_t* vm);
void Mul      (processor_t* vm);
void Div      (processor_t* vm);
void Away     (processor_t* vm);
void Hlt      (processor_t* vm);
void CmdSwitch(processor_t* vm);
void Pop_reg(processor_t* vm, int reg_id);
void Push_reg(processor_t* vm, int reg_id);



void   ReadByteCode (const char* name,   int* code);
int    CountLines   (char*       buffer, size_t size);
size_t GetSize      (const char* name);
void   FillCodeArray(int*        code,   char* buffer, int lines_count);


const char code_file[] = "code.txt";

int main()
{
    processor_t vm = {};
    StackInit(&(vm.stk), 8);
    ReadByteCode(code_file, vm.code);

    CmdSwitch(&vm);
    return 0;
}

void ReadByteCode(const char* name, int* code)
{
    size_t size = GetSize(name);
    FILE* file = fopen(name, "r");
    char* buffer = (char*)calloc(size + 1, sizeof(char));
    fread(buffer, sizeof(char), size, file);
    int lines_count = CountLines(buffer, size);

    FillCodeArray(code, buffer, lines_count);
    fclose(file);
    free(buffer);
}

void FillCodeArray(int* code, char* buffer, int lines_count)
{
    int cmd_number = 0;
    int number = 0;
    int buf_shift = 0;
    int correct_args = 0;
    int current_line = 0;
    int args = 0;
    while (current_line < lines_count)
    {
        correct_args = sscanf(buffer + buf_shift, "%d%d", &cmd_number, &number);
        code[current_line + args] = cmd_number;
        if (correct_args == 2)
        {
            args++;
            code[current_line + args] = number;
        }
        while (*(buffer + buf_shift) != '\0')
        {
            buf_shift++;
        }
        buf_shift++;
        current_line++;
    }
}

int CountLines(char* buffer, size_t size)
{
    char sym = 0;
    int line_count = 0;
    for (int buf_idx = 0; buf_idx < (int)size; buf_idx++)
    {
        sym = *(buffer + buf_idx);
        assert(sym != '\r');
        if (sym == '\n')
        {
            line_count++;
            *(buffer + buf_idx) = '\0';
        }
    }
    return line_count;
}

size_t GetSize(const char* name)
{
    struct stat st;
    if (stat(name, &st) != 0)
    {
        fprintf(stderr, "ERROR");
        return -1;
    }
    return st.st_size;
}

void Push(processor_t* vm, int value)
{
    StackPush(&(vm->stk), value);
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
    printf("%d\n", value);
}

void Hlt(processor_t* vm)
{
    StackDestroy(&(vm->stk));
}

void Pop_reg(processor_t* vm, int reg_id)
{
    StackPop(&(vm->stk), vm->regs + reg_id - 1);
}

void Push_reg(processor_t* vm, int reg_id)
{
    StackPush(&(vm->stk), vm->regs[reg_id - 1]);
}

void CmdSwitch(processor_t* vm)
{
    int ip = 0;
    bool no_hlt = 1;
    while (no_hlt)
    {
        switch(vm->code[ip])
        {
            case PUSH:
                ip++;
                Push(vm, vm->code[ip]);
                break;
            case POPr:
                ip++;
                Pop_reg(vm, vm->code[ip]);
                break;
            case PUSHr:
                ip++;
                Push_reg(vm, vm->code[ip]);
                break;
            case ADD:
                Add(vm);
                break;
            case SUB:
                Sub(vm);
                break;
            case DIV:
                Div(vm);
                break;
            case AWAY:
                Away(vm);
                break;
            case HLT:
                Hlt(vm);
                no_hlt = 0;
                break;
            case MUL:
                Mul(vm);
                break;
        }
        ip++;
    }
}
