#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#define PROCESSOR

#include "stack.h"
#include "core.h"
#include "../enum.h"
#include "readswitch.h"

void ReadByteCode(const char* name, processor_t* vm) // getsize countlines fillcodearray
{
    size_t size = GetSize(name);
    FILE* file = fopen(name, "r");
    char* buffer = (char*)calloc(size + 1, sizeof(char));
    fread(buffer, sizeof(char), size, file);

    int lines_count = CountLines(buffer, size);
    FillCodeArray(vm, buffer, lines_count);

    fclose(file);
    free(buffer);
}

void FillCodeArray(processor_t* vm, char* buffer, int lines_count)
{
    int cmd_id = 0; //
    int number = 0;
    int buf_shift = 0;
    int correct_args = 0;
    int current_line = 0;
    int args = 0;
    while (current_line < lines_count)
    {
        correct_args = sscanf(buffer + buf_shift, "%d%d", &cmd_id, &number);
        (vm->code)[current_line + args] = cmd_id;

        if (correct_args == 2)
        {
            args++;
            (vm->code)[current_line + args] = number;
        }
        while (*(buffer + buf_shift) != '\0')
            buf_shift++;

        buf_shift++;
        current_line++;
    }
    vm->last_idx = current_line + args;
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

void CmdSwitch(processor_t* vm, commands_str* cmd_array) //TODO: struct.h
{
    int index = 0;
    while(vm->code[vm->ip] != HLT) // идем по массиву с кодом
    {
        for (index = 0; index < CMD_AMOUNT; index++) // сравниваем номера команд
        {
            if (cmd_array[index].id == vm->code[vm->ip])
            {
                if (cmd_array[index].arg_number == 1) // есть аргумент
                    vm->ip++;

                cmd_array[index].Function(vm);
                break;
            }
        }
        vm->ip++;
    }
}
