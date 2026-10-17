#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>

enum cmd {
    HLT,
    PUSH,
    ADD,
    SUB,
    DIV,
    OUT,
    MUL
};

int    ReadFromFile  (const char* name,        char*** index_array, char** buffer);
void   WriteInFile(const char* name, char** index_array, int lines_count);
void   FillIndexArray(char***     index_array, char**  buffer,      size_t size);
int    CountLines    (char**      buffer,      size_t  size);
size_t GetSize       (const char* name);
void   PrintPointerArr (char** arr, int leng);
int    GetCmdNumber(char* cmd);

int main()
{
    char* buffer = 0;
    char** index_array = 0;
    const char source[] = "code.asm";
    const char bite[] = "code.txt";
    // printf("456789\n");
    int lines_count = ReadFromFile(source, &index_array, &buffer);
    printf("22\n");
    if (lines_count == -1)
    {
        printf("Error\n");
    }
    WriteInFile(bite, index_array, lines_count);
    free(index_array);
    free(buffer);
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
    if (strcmp(cmd, "OUT") == 0)
        return OUT;
    if (strcmp(cmd, "MUL") == 0)
        return MUL;
    else
        return 0;
}

int ReadFromFile(const char* name, char*** index_array, char** buffer)
{
    // запись размера файла
    size_t size = GetSize(name);
    // открытие файла
    FILE* file = fopen(name, "r");
    if (file == 0)
    {
        printf("Error: calloc returned 0 in file: %d in line %d", __FILE__, __LINE__);
        return -1;
    }
    // создание буфера и массива с указателями
    *buffer = (char*)calloc(size + 1, sizeof(char));
    if (*buffer == 0)
    {
        printf("Error: calloc returned 0 in file: %d in line %d", __FILE__, __LINE__);
        return -1;
    }
    // заполнение буфера
    fread(*buffer, sizeof(char), size, file);
    // подсчет строк в файле
    int lines_count = CountLines(buffer, size);
    *index_array = (char**)calloc(lines_count + 1, sizeof(char*));
    if (*index_array == 0)
    {
        printf("Error: calloc returned 0 in file: %d in line %d", __FILE__, __LINE__);
        return -1;
    }
    // заполнение массива с указателями
    FillIndexArray(index_array, buffer, size);
    // закрытие файла
    fclose(file);
    return lines_count;
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

int CountLines(char** buffer, size_t size)
{
    char sym = 0;
    int line_count = 0;
    for (long long buf_idx = 0; buf_idx < size; buf_idx++)
    {
        sym = *(*buffer + buf_idx);
        assert(sym != '\r');
        if (sym == '\n')
        {
            line_count++;
            *(*buffer + buf_idx) = '\0';
        }
    }
    return line_count;
}

void FillIndexArray(char*** index_array, char** buffer, size_t size)
{
    char sym = 0;
    (*index_array)[0] = *buffer;
    int line_idx = 1;
    for (long long buf_idx = 1; buf_idx < size; buf_idx++)
    {
        sym = *(*buffer + buf_idx);

        if (sym == '\0')
        {
            (*index_array)[line_idx] = *buffer + buf_idx + 1;
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
    char string[100] = {};
    int number = 0;
    int cmd_number = 0;
    int correct_args = 0;
    for (int line = 0; line < lines_count; line++)
    {
        correct_args = sscanf(index_array[line], "%s%d", string, &number);
        cmd_number = GetCmdNumber(string);
        fprintf(file, "%d", cmd_number);
        if (correct_args == 2)
        {
            fprintf(file, " %d", number);
        }
        putc('\n', file);
    }
    fclose(file);
}
