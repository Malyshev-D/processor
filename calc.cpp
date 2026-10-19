#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include "stack.h"

enum cmd {
    HLT,
    PUSH,
    ADD,
    SUB,
    DIV,
    OUT,
    MUL
};

void Push     (stack_t* stk, int value);
void Add      (stack_t* stk);
void Sub      (stack_t* stk);
void Mul      (stack_t* stk);
void Div      (stack_t* stk);
void Out      (stack_t* stk);
void Hlt      (stack_t* stk);
void CmdSwitch(stack_t* stk, int* code);

void   ReadByteCode (const char* name,   int* code);
int    CountLines   (char*       buffer, size_t size);
size_t GetSize      (const char* name);
void   FillCodeArray(int*        code,   char* buffer, int lines_count);

int main()
{
    stack_t stk = {};
    StackInit(&stk, 8);
    int code[1000] = {};
    const char byte[] = "code.txt";
    ReadByteCode(byte, code);

    CmdSwitch(&stk, code);
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
    for (int buf_idx = 0; buf_idx < size; buf_idx++)
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

void Push(stack_t* stk, int value)
{
    StackPush(stk, value);
}

void Add(stack_t* stk)
{
    int value1, value2;
    StackPop(stk, &value1);
    StackPop(stk, &value2);
    StackPush(stk, value1 + value2);
}

void Sub(stack_t* stk)
{
    int value1, value2;
    StackPop(stk, &value1);
    StackPop(stk, &value2);
    StackPush(stk, value2 - value1);
}

void Mul(stack_t* stk)
{
    int value1, value2;
    StackPop(stk, &value1);
    StackPop(stk, &value2);
    StackPush(stk, value1 * value2);
}

void Div(stack_t* stk)
{
    int value1, value2;
    StackPop(stk, &value1);
    StackPop(stk, &value2);
    StackPush(stk, value2 / value1);
}

void Out(stack_t* stk)
{
    int value;
    StackPop(stk, &value);
    printf("%d", value);
}

void Hlt(stack_t* stk)
{
    StackDestroy(stk);
}

void CmdSwitch(stack_t* stk, int* code)
{
    int ip = 0;
    bool no_hlt = 1;
    while (no_hlt)
    {
        switch(code[ip])
        {
            case PUSH:
                ip++;
                Push(stk, code[ip]);
                break;
            case ADD:
                Add(stk);
                break;
            case SUB:
                Sub(stk);
                break;
            case DIV:
                Div(stk);
                break;
            case OUT:
                Out(stk);
                break;
            case HLT:
                Hlt(stk);
                no_hlt = 0;
                break;
            case MUL:
                Mul(stk);
                break;
        }
        ip++;
    }
}
