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
    int ip;
};

void Push            (processor_t* vm, int value);
void Add             (processor_t* vm);
void Sub             (processor_t* vm);
void Mul             (processor_t* vm);
void Div             (processor_t* vm);
void Away            (processor_t* vm);
void Hlt             (processor_t* vm);
void Pop_reg         (processor_t* vm, int reg_id);
void Push_reg        (processor_t* vm, int reg_id);
void Jump            (processor_t* vm, int label_idx);
void JumpIfAbove     (processor_t* vm, int label_idx);
void JumpIfAboveEqual(processor_t* vm, int label_idx);
void JumpIfBelow     (processor_t* vm, int label_idx);
void JumpIfBelowEqual(processor_t* vm, int label_idx);
void JumpIfEqual     (processor_t* vm, int label_idx);
void JumpIfNotEqual  (processor_t* vm, int label_idx);

void CmdSwitch       (processor_t* vm);
void   ReadByteCode  (const char*  name,   int*  code);
int    CountLines    (char*        buffer, size_t size);
size_t GetSize       (const char*  name);
void   FillCodeArray (int*         code,   char*  buffer, int lines_count);

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

void Jump(processor_t* vm, int label_idx)
{
    vm->ip = label_idx - 1;
}

void JumpIfAbove(processor_t* vm, int label_idx)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a > b)
        Jump(vm, label_idx);

}

void JumpIfAboveEqual(processor_t* vm, int label_idx)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a >= b)
        Jump(vm, label_idx);

}

void JumpIfBelow(processor_t* vm, int label_idx)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a < b)
        Jump(vm, label_idx);

}

void JumpIfBelowEqual(processor_t* vm, int label_idx)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a <= b)
        Jump(vm, label_idx);

}

void JumpIfEqual(processor_t* vm, int label_idx)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a == b)
        Jump(vm, label_idx);

}

void JumpIfNotEqual(processor_t* vm, int label_idx)
{
    int a, b;
    StackPop(&(vm->stk), &a);
    StackPop(&(vm->stk), &b);
    if (a != b)
        Jump(vm, label_idx);

}

void CmdSwitch(processor_t* vm)
{
    bool no_hlt = 1;
    while (no_hlt)
    {
        switch(vm->code[vm->ip])
        {
            case PUSH:
                vm->ip++;
                Push(vm, vm->code[vm->ip]);
                break;
            case POPr:
                vm->ip++;
                Pop_reg(vm, vm->code[vm->ip]);
                break;
            case PUSHr:
                vm->ip++;
                Push_reg(vm, vm->code[vm->ip]);
                break;
            case JMP:
                vm->ip++;
                Jump(vm, vm->code[vm->ip]);
                break;
            case JA:
                vm->ip++;
                JumpIfAbove(vm, vm->code[vm->ip]);
                break;
            case JAE:
                vm->ip++;
                JumpIfAboveEqual(vm, vm->code[vm->ip]);
                break;
            case JB:
                vm->ip++;
                JumpIfBelow(vm, vm->code[vm->ip]);
                break;
            case JBE:
                vm->ip++;
                JumpIfBelowEqual(vm, vm->code[vm->ip]);
                break;
            case JE:
                vm->ip++;
                JumpIfEqual(vm, vm->code[vm->ip]);
                break;
            case JNE:
                vm->ip++;
                JumpIfNotEqual(vm, vm->code[vm->ip]);
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
        vm->ip++;
    }
}
