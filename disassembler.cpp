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

int         ReadFromFile   (const char* name,        char*** index_array, char** buffer, int* lines_count);
void        WriteInFile    (const char* name,        char**  index_array, int    lines_count);
void        FillIndexArray (char**      index_array, char*   buffer,      size_t size);
int         CountLines     (char*       buffer,      size_t  size);
size_t      GetSize        (const char* name);
void        PrintPointerArr(char**      arr,         int leng);
const char* NumberToCmd    (int         number);

int main()
{
    char* buffer = 0;
    char** index_array = 0;
    const char source[] = "code.txt";
    const char dest[] = "code1.asm";
    int lines_count = 0;

    int error = ReadFromFile(source, &index_array, &buffer, &lines_count);
    if (error != 0)
    {
        printf("Error\n");
        free(index_array);
        free(buffer);
        return 0;
    }
    WriteInFile(dest, index_array, lines_count);
    free(index_array);
    free(buffer);
    return 0;
}


const char* NumberToCmd(int number)
{
    switch (number)
    {
        case HLT:
            return "HLT";
        case PUSH:
            return "PUSH";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case DIV:
            return "DIV";
        case OUT:
            return "OUT";
        case MUL:
            return "MUL";
        default:
            return "UNK";
    }
    printf("finish\n");
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
    int cmd_number = 0;
    int number = 0;
    int correct_args = 0;
    for (int line = 0; line < lines_count; line++)
    {
        correct_args = sscanf(index_array[line], "%d%d", &cmd_number, &number);
        fprintf(file, NumberToCmd(cmd_number));
        if (correct_args == 2)
        {
            fprintf(file, " %d", number);
        }
        putc('\n', file);
    }
    fclose(file);
}
