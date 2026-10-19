#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

enum error_value StackInit (stack_t* stk, int capacity ON_DEBUG(,const char* name, const char* file, int line))
{
    if (stk == 0)
        return ZEROSTACK;

    ON_DEBUG(
    stk->stack_canary1 = STACKCANARY1;
    stk->fulldata = (StackElem_t*)calloc(capacity + 2, elem_size);
    if (stk->fulldata == nullptr)
        return ZERODATA;

    stk->fulldata[0] = CANARY1;
    stk->data    = stk->fulldata + 1;
    stk->name    = name;
    stk->file    = file;
    stk->line    = line;
    stk->error   = NOERROR;
    stk->fulldata[capacity + 2] = CANARY2;
    stk->capacity = capacity;
    StackPoison(stk, 0, capacity);
    stk->stack_canary2 = STACKCANARY2;
    return StackVerify(stk);
    )

    stk->data = (StackElem_t*)calloc(capacity, elem_size);
    if (stk->data == nullptr)
        return ZERODATA;
    stk->capacity = capacity;

    StackPoison(stk, 0, capacity);
    return NOERROR;
}

enum error_value StackPush(stack_t* stk, StackElem_t value)
{
    ON_DEBUG(
    if (StackVerify(stk) != NOERROR)
        return stk->error;
    )

    if (stk->size == stk->capacity)
        StackResize(stk, stk->capacity * 2);

    stk->data[stk->size] = value;
    stk->size++;

    ON_DEBUG(return StackVerify(stk);)
    return NOERROR;
}

enum error_value StackPop(stack_t* stk, StackElem_t* value)
{
    ON_DEBUG(
    if (StackVerify(stk) != NOERROR)
        return stk->error;
    )
    stk->size--;
    *value = stk->data[stk->size];
    stk->data[stk->size] = POISON;

    ON_DEBUG(return StackVerify(stk);)
    return NOERROR;
}

void StackDestroy(stack_t* stk)
{
    if (stk != nullptr && stk->data != nullptr)
    {
        stk->size = stk->capacity = 0;
        free(stk->data);
    }
}

ON_DEBUG(
enum error_value StackVerify(stack_t* stk)
{
    // ошибка найдена до вызова
    if (stk->error != NOERROR)
    {
        StackDump(*stk);
        return stk->error;
    }
    // диагностика ошибок
    if (stk == nullptr)
        stk->error = ZEROSTACK;

    if (stk->data == nullptr)
        stk->error = ZERODATA;

    if (stk->size < 0)
        stk->error = NEGSIZE;

    if (stk->capacity < 0)
        stk->error = NEGCAPACITY;

    if (stk->size > stk->capacity)
        stk->error = OVERFLOW;

    if (stk->fulldata[0] != CANARY1 || stk->fulldata[stk->capacity + 2] != CANARY2)
        stk->error = DATA_CANARY_DIED;

    if (stk->stack_canary1 != STACKCANARY1 || stk->stack_canary2 != STACKCANARY2)
        stk->error = STK_CANARY_DIED;
    // вывод ошибки при наличии
    if (stk->error != NOERROR)
        StackDump(*stk);
    return stk->error;
})

void StackDump(stack_t stk)
{
    ON_DEBUG(
    printf(ANSI_COLOR_RED);
    switch(stk.error)
    {
        case ZEROSTACK:
            printf("STACK POINTER IS NULL\n");
        case ZERODATA:
            printf("DATA POINTER IS NULL\n");
            break;
        case NEGSIZE:
            printf("SIZE IS LESS THAN ZERO\n");
            break;
        case NEGCAPACITY:
            printf("CAPACITY IS LESS THAN ZERO\n");
            break;
        case OVERFLOW:
            printf("CAPACITY IS LESS THAN SIZE\n");
            break;
        case DATA_CANARY_DIED:
            printf("PTICHKU ZHALKO (in data)\n"
            "CANARY1 = <%llX> CANARY1 = <%llX>\n"
            "SHOULD BE:\n"
            "CANARY1 = <%llX> CANARY1 = <%llX>\n",
            (long long)stk.fulldata[0], (long long)stk.fulldata[stk.capacity + 2],
            CANARY1, CANARY2);
            break;
        case STK_CANARY_DIED:
            printf("PTICHKU ZHALKO(in stack)\n"
            "CANARY1 = <%llX> CANARY1 = <%llX>\n"
            "SHOULD BE:\n"
            "CANARY1 = <%llX> CANARY1 = <%llX>\n",
            (long long)stk.stack_canary1, (long long)stk.stack_canary2,
            STACKCANARY1, STACKCANARY2);
            break;
    }
    printf(ANSI_COLOR_YELLOW "Stack \"%s\" [%p] created at %s:%d\n" ANSI_COLOR_RESET,
    stk.name, stk.data, stk.file, stk.line);
    )
    StackSimPrint(stk);
}

enum error_value StackResize(stack_t* stk, int newcapacity)
{
    ON_DEBUG(
    if (StackVerify(stk) != NOERROR)
        return stk->error;)

    int oldcapacity = stk->capacity;

    #ifdef STACK_DEBUG

    stk->fulldata[oldcapacity + 2] = POISON;
    stk->fulldata = (StackElem_t*)realloc((void*)(stk->fulldata), ((newcapacity  + 2) * elem_size));
    if (stk->fulldata == nullptr)
        return ZERODATA;
    stk->data = stk->fulldata + 1;

    #else

    stk->data = (StackElem_t*)realloc((void*)(stk->data), (newcapacity * elem_size));
    if (stk->data == nullptr)
        return ZERODATA;

    #endif

    stk->capacity = newcapacity;
    if (oldcapacity < newcapacity)
        StackPoison(stk, oldcapacity + 1, newcapacity);

    ON_DEBUG(
    stk->fulldata[newcapacity + 2] = CANARY2;
    return StackVerify(stk);
    )
    return NOERROR;
}

void StackPoison(stack_t* stk, int first_idx, int last_idx)
{
    for (int idx = first_idx; idx < last_idx; idx++)
        stk->data[idx] = POISON;
}

void StackSimPrint(stack_t stk)
{
    printf("elements:");
    for (int idx = 0; idx < stk.capacity; idx++)
    {
        if (idx % 10 == 0)
        {
            putchar('\n');
        }
        printf("<%.4lg> ", stk.data[idx]);
    }
    printf("\nsize: %d\ncapacity: %d\n\n",
    stk.size, stk.capacity);
}
