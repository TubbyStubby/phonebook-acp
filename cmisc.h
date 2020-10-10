#ifndef CMISC_H
#define CMISC_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "helper.h"
#define pchar(x) printf("%c", x)

//stores form data which can either be string or integer
typedef struct
{
    Type t;
    Value data;
} FormData;

//structure for box characters
typedef struct
{
    char h;
    char v;
    char tl;
    char tr;
    char br;
    char bl;
} Box;

//COORD contains 2 ints X and Y

//self explanatory
void gotoxy(COORD);

COORD wherexy(void);

/*
returns size of window
note positions in window starts from 0,0
*/
COORD getWindowSize(void);

/*
draws a box with given width and height
at given coord
note: w, h should be atleast 3 to get a usable space of 1 block
*/
void drawBox(const Box*, int, int, COORD);

//just for fun hehe and testing
void freeRoam(void);

/*
Pending
    takes 2d array consisting of multiple strings which are labels
    int which is no. of labels
    and coords where the form is to be made
*/
FormData* form(char**, int, COORD);

/*
return string ending with \n
without going to new line
*/
void xinput(char*);

//standard box with double line
extern const Box dlBox;
//standard box with single line
extern const Box slBox;

#endif //CMISC_H

//enter is 13 with _getch