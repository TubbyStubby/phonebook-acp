#ifndef HELPER_H
#define HELPER_H

typedef enum { str, num } Type;

typedef union
{
    char s[20];
    int i;
} Value;

//todo: need to make it generic using null*
typedef struct DLL
{
    struct DLL *next;
    struct DLL *prev;
} DLL;

#endif //HELPER_H