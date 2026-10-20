#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "enum.h"

int    ReadFromFile   (const char* name,        char*** index_array, char** buffer, int* lines_count);
void   WriteInFile    (const char* name,        char**  index_array, int    lines_count);
void   FillIndexArray (char**      index_array, char*   buffer,      size_t size);
int    CountLines     (char*       buffer,      size_t  size);
size_t GetSize        (const char* name);
void   PrintPointerArr(char**      arr,         int     leng);
int    GetCmdNumber   (char*       cmd);
void   TranslateArg   (FILE*       file,        char**  index_array, int    line,   int  index, int shift);
void   StringToZero   (char*       arr,         int     leng);

const char source[] = "code.asm";
const char dest[] = "code.txt";

int main()
{
    char* buffer = 0;
    char** index_array = 0;

    int lines_count = 0;
    int error = ReadFromFile(source, &index_array, &buffer, &lines_count);
    // fprintf(stderr, "Reading completed\n");
    // PrintPointerArr(index_array, lines_count);
    if (error != 0)
    {
        fprintf(stderr, "Error\n");
        free(index_array);
        free(buffer);
        return 0;
    }
    WriteInFile(dest, index_array, lines_count);
    free(index_array);
    free(buffer);
    return 0;
}


int GetCmdNumber(char* cmd)
{
    if (strcmp(cmd, "HLT") == 0)
        return HLT;
    if (strcmp(cmd, "PUSH") == 0)
        return PUSH;
    if (strcmp(cmd, "ADD") == 0)
        return ADD;
    if (strcmp(cmd, "SUB") == 0)
        return SUB;
    if (strcmp(cmd, "DIV") == 0)
        return DIV;
    if (strcmp(cmd, "AWAY") == 0)
        return AWAY;
    if (strcmp(cmd, "MUL") == 0)
        return MUL;
    if (strcmp(cmd, "PUSHr") == 0)
        return PUSHr;
    if (strcmp(cmd, "POPr") == 0)
        return POPr;
    if (strcmp(cmd, "AX") == 0)
        return AX;
    if (strcmp(cmd, "BX") == 0)
        return BX;
    if (strcmp(cmd, "CX") == 0)
        return CX;
    if (strcmp(cmd, "DX") == 0)
        return DX;
    else
        return 0;
}

int ReadFromFile(const char* name, char*** index_array, char** buffer, int* lines_count)
{
    // запись размера файла
    size_t size = GetSize(name);
    // открытие файла
    FILE* file = fopen(name, "rb");
    if (file == 0)
    {
        printf("Error: calloc returned 0 in file: %s in line %d", __FILE__, __LINE__);
        return -1;
    }
    // создание буфера
    *buffer = (char*)calloc(size + 1, sizeof(char));
    if (*buffer == 0)
    {
        printf("Error: calloc returned 0 in file: %s in line %d", __FILE__, __LINE__);
        return -1;
    }
    // заполнение буфера
    fread(*buffer, sizeof(char), size, file);
    // подсчет строк в файле
    *lines_count = CountLines(*buffer, size);
    // создание массива с указателями
    *index_array = (char**)calloc(*lines_count + 1, sizeof(char*));
    if (*index_array == 0)
    {
        printf("Error: calloc returned 0 in file: %s in line %d", __FILE__, __LINE__);
        return -1;
    }
    // заполнение массива с указателями
    FillIndexArray(*index_array, *buffer, size);
    // закрытие файла
    fclose(file);
    return 0;
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

int CountLines(char* buffer, size_t size)
{
    char sym = 0;
    int line_count = 0;
    for (int buf_idx = 0; buf_idx < (int)size; buf_idx++)
    {
        sym = *(buffer + buf_idx);
        if (sym == '\r')
            *(buffer + buf_idx) = '\0';

        if (sym == '\n')
        {
            line_count++;
            *(buffer + buf_idx) = '\0';
        }
    }
    return line_count;
}

void FillIndexArray(char** index_array, char* buffer, size_t size)
{
    char sym = 0;
    index_array[0] = buffer;
    int line_idx = 1;
    for (int buf_idx = 1; buf_idx < (int)size; buf_idx++)
    {
        sym = *(buffer + buf_idx);

        if (sym == '\0' && *(buffer + buf_idx + 1) == '\0')
        {
            index_array[line_idx] = buffer + buf_idx + 2;
            line_idx++;
        }
    }
}

void PrintPointerArr (char** arr, int leng)
{
    for (int ind = 0; ind < leng; ind++)
        puts(arr[ind]);
}

void WriteInFile(const char* name, char** index_array, int lines_count)
{
    FILE* file = fopen(name, "w");
    char cmd[20] = {};
    int index = 0;
    int shift = 0;
    for (int line = 0; line < lines_count; line++)
    {
        index = 0;
        shift = 0;
        StringToZero(cmd, 20);
        while (index_array[line][index] != ' ' && index_array[line][index] != '\0')
        {
            cmd[index] = index_array[line][index];
            // printf("line <%d>, index <%d>, symbol <%c>, cmd <%s>\n", line, index, index_array[line][index], cmd);
            shift++;
            index++;
        }
        fprintf(file, "%d", GetCmdNumber(cmd));
        // printf("%c", word[i]);
        // printf("line <%d>, index <%d>, symbol <%c>\n", line, index, index_array[line][index]);
        if (index_array[line][index] == ' ')
            TranslateArg(file, index_array, line, index, shift);
        putc('\n', file);

    }
    fclose(file);
}

void TranslateArg(FILE* file, char** index_array, int line, int index, int shift)
{
    char arg[10] = {};
    int is_number = 0;
    int number = 0;
    index++;
    shift++;
    // printf("line <%d>, index <%d>, symbol <%c>\n", line, index, index_array[line][index]);
    is_number = sscanf(index_array[line] + index, "%d", &number);
    // printf("number <%d>\n", number);
    if (is_number != 1)
    {
        while (index_array[line][index] != '\0')
        {
            arg[index - shift] = index_array[line][index];
            index++;
            // printf("line <%d>, index <%d>, symbol <%c>\n", line, index, index_array[line][index]);
        }
        number = GetCmdNumber(arg);
        // printf("line <%d>, index <%d>, symbol <%c>\n", line, index, index_array[line][index]);
    }
    fprintf(file, " %d", number);
}

void StringToZero(char* arr, int leng)
{
    for (int i = 0; i < leng; i++)
    {
        arr[i] = 0;
    }
}
