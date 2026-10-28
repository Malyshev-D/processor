// #include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>

#include "reading.h"
#include "writing.h"

const int  LABELS_RANGE = 15;
const char source[] = "../square.asm";
const char dest[] = "../square.txt";

int main()
{
    char* buffer = NULL;
    char** index_array = NULL;
    int labels[LABELS_RANGE] = {};
    SetLabels(labels, LABELS_RANGE);
    int lines_count = 0;
    int error = ReadFromFile(source, &index_array, &buffer, &lines_count);
    if (error != 0)
    {
        fprintf(stderr, "Error\n");
        free(index_array);
        free(buffer);
        return 0;
    }
    WriteInFile(dest, index_array, lines_count, labels);

    free(index_array);
    free(buffer);
    return 0;
}
