#ifndef HELPER_H
#define HELPER_H

typedef enum { str, num } Type;

typedef union
{
    char s[20];
    int i;
} Value;

#endif //HELPER_H