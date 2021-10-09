#include<stdio.h>
#include<stdlib.h>

typedef enum { str, num } Type;

typedef union 
{
    char *s;
    int i;
} Value;

typedef struct Data
{
    Type t;
    Value v;
} Data;

int main()
{
    Data *d = (Data*)malloc(sizeof(Data)*2);
    d[0].t = str;
    d[0].v.s = (char*)malloc(sizeof(char)*20);
    scanf("%s", d[0].v.s);
    printf(d[0].v.s);
    d[1].t = num;
    scanf("%d", &(d[1].v.i));
    printf("%s %d", d[0].v.s, d[1].v.i);
}