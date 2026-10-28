#ifndef WRITE_IN_FILE
#define WRITE_IN_FILE

void WriteInFile (const char* name,   char** index_array, int lines_count, int* labels);
void FillLabels  (const char* name,   char** index_array, int lines_count, int* labels);
int  GetCmdNumber(char*       cmd);
int  GetRegister (char*       cmd);
int  TranslateCmd(char*       string, char*  cmd);
int  TranslateArg(char*       word,   int*   labels,      int shift);
void StringReset (char*       arr,    int    leng);

#endif
