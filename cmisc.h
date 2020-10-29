#ifndef CMISC_H
#define CMISC_H

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "helper.h"
#define pchar(x) printf("%c", x)
#define _cord(x,y) (COORD){x,y} //short hand for new COORD
#define arrow(x, y) gotoxy(_cord(x,y));putch('>');gotoxy(_cord(x,y))

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

/*
clears the box of given width and height
*/
void clearBox(int, int, COORD);

/*
clears the area from current pos to the right and bottom boundaries
*/
void clearArea(COORD);

//just for fun hehe and testing
void freeRoam(void);

/*
takes 2d array consisting of multiple strings which are labels
int which is no. of labels
and coords where the form is to be made
*/
FormData* form(char**, int, COORD); //change to formdata

/*
return string ending with \n
without going to new line
*/
int xinput(char*);

//makes a vertical line
void vLine(COORD, int);
//makes a horizontal line
void hLine(COORD, int);

//makes a arrow selectable menu
int aroSelect(char**, int);

//standard box with double line
extern const Box dlBox;
//standard box with single line
extern const Box slBox;

#endif //CMISC_H
