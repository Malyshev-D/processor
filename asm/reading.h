#ifndef READING_FILE
#define READING_FILE

int    ReadFromFile   (const char* name,        char*** index_array, char** buffer,      int* lines_count);
void   FillIndexArray (char**      index_array, char*   buffer,      size_t size);
int    CountLines     (char*       buffer,      size_t  size);
size_t GetSize        (const char* name);
void   PrintPointerArr(char**      arr,         int     leng);
void   SetLabels      (int*        labels,      int     leng);

#endif
