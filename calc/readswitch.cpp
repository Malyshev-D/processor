#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#define PROCESSOR

#include "stack.h"
#include "core.h"
#include "../enum.h"
#include "readswitch.h"

// чтение байт-кода и запись в массив
void ReadByteCode(const char* name, processor_t* vm)
{
    // открываем файл, создаем буфер
    size_t size = GetSize(name);
    FILE* file = fopen(name, "r");
    char* buffer = (char*)calloc(size + 1, sizeof(char));
    fread(buffer, sizeof(char), size, file);
    // заполняем массив с кодом
    int lines_count = CountLines(buffer, size);
    FillCodeArray(vm, buffer, lines_count);

    fclose(file);
    free(buffer);
}

// заполнение массива с кодом
void FillCodeArray(processor_t* vm, char* buffer, int lines_count)
{
    int cmd_id = 0;
    int arg_value = 0;
    int buf_shift = 0;
    int correct_args = 0;
    int current_line = 0;
    int args_amount = 0;
    while (current_line < lines_count)
    {
        // подсчет кол-ва чисел в строке
        correct_args = sscanf(buffer + buf_shift, "%d%d", &cmd_id, &arg_value);
        (vm->code)[current_line + args_amount] = cmd_id;

        if (correct_args == 2)
        {
            args_amount++;
            (vm->code)[current_line + args_amount] = arg_value;
        }
        while (*(buffer + buf_shift) != '\0')
            buf_shift++;

        buf_shift++;
        current_line++;
    }
    vm->last_idx = current_line + args_amount;
}

// подсчет строк и замена \r\n на \0\0
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

// получение размера читаемого файла
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

// чтение массива с кодом и выполнение команд
void CmdSwitch(processor_t* vm, commands_str* cmd_array) //DONE TODO: struct.h
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
        // StackDump(vm->stk);

        vm->ip++;
        // printf("ip = [%d], cmd = [%d]\n", vm->ip, vm->code[vm->ip]);

    }
}
