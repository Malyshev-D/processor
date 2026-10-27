#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>

#define CMDARRAY
#define REGISTER_ARRAY

#include "../enum.h"
#include "writing.h"

int GetCmdNumber(char* cmd)
{
    int index = 0;
    for (index = 0; index < CMD_AMOUNT; index++)
    {

        if (strcmp(cmd_array[index].word, cmd) == 0)
            return cmd_array[index].id;
    }
    for (index = 0; index < REG_AMOUNT; index++)
    {
        if (strcmp(reg_array[index].word, cmd) == 0)
            return reg_array[index].id;
    }

    return 0;
}

void WriteInFile(const char* name, char** index_array, int lines_count, int* labels)
{
    FillLabels(name, index_array, lines_count, labels);
    FillLabels(name, index_array, lines_count, labels);
}

void FillLabels(const char* name, char** index_array, int lines_count, int* labels)
{
    FILE* file = fopen(name, "w");

    char cmd[20] = {};
    int shift = 0;
    int label_idx = -1;
    int args = 0;
    for (int line = 0; line < lines_count; line++)
    {
        shift = 0;
        StringReset(cmd, 20);
        if (index_array[line][shift] == ':') // проверка на метку
        {
            sscanf(index_array[line] + 1, "%d", &label_idx);
            if (labels[label_idx] == -1)
                labels[label_idx] = line + args;
        }
        else
        {   // если обычная команда
            shift = TranslateCmd(index_array[line], cmd);
            fprintf(file, "%d", GetCmdNumber(cmd));
            // printf("line <%d>, index <%d>, symbol <%c>\n", line, index, index_array[line][index]);
            if (index_array[line][shift] == ' ')
            {
                fprintf(file, " %d", TranslateArg(index_array[line], labels, shift + 1));
                args++;
            }
            putc('\n', file);
        }
    }
    fclose(file);
}

int TranslateCmd(char* string, char* cmd)
{
    int index = 0;
    while (string[index] != ' ' && string[index] != '\0')
    {
        cmd[index] = string[index];
        index++;
    }
    return index;
}

int TranslateArg(char* word, int* labels, int shift)
{
    char argument[10] = {};
    int is_number = 0;
    int number = 0;
    int label_idx = -1;
    int index = shift;
    if (word[index] == ':')
    {
        sscanf(word + index + 1, "%d", &label_idx);
        number = labels[label_idx];
    }
    else
    {
        is_number = sscanf(word + index, "%d", &number);
        if (is_number != 1)
        {
            while (word[index] != '\0')
            {
                argument[index - shift] = word[index];
                index++;
            }
            number = GetCmdNumber(argument);
        }
    }
    return number;
}

void StringReset(char* arr, int leng)
{
    for (int i = 0; i < leng; i++)
    {
        arr[i] = 0;
    }
}
