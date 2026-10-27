#ifndef FILEWORK
#define FILEWORK

void   ReadByteCode (const char*  name,   processor_t*  vm);
int    CountLines   (char*        buffer, size_t        size);
size_t GetSize      (const char*  name);
void   FillCodeArray(processor_t* vm,     char*         buffer, int lines_count);
void   CmdSwitch    (processor_t* vm,     commands_str* cmd_array);

#endif
