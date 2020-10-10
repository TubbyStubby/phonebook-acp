#include "cmisc.h"

//common console buffer
CONSOLE_SCREEN_BUFFER_INFO csbi;

const Box dlBox = {205, 186, 201, 187, 188, 200};
const Box slBox = {196, 179, 218, 191, 217, 192};

void gotoxy(COORD cord) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord); }

COORD wherexy()
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
}

COORD getWindowSize()
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD cord;
    cord.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    cord.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return cord;
}

void drawBox(const Box* box, int w, int h, COORD stpos)
{
    gotoxy(stpos);
    COORD cc = stpos;
    short dir=1, axs=0;
    do
    {
        gotoxy(cc);
        if(cc.X == stpos.X && cc.Y == stpos.Y) pchar(box->tl);
        else if(cc.X == stpos.X + w - 1 && cc.Y == stpos.Y) 
        {
            pchar(box->tr);
            axs = 1;
        }
        else if(cc.X == stpos.X && cc.Y == stpos.Y + h -1) 
        {
            pchar(box->bl);
            axs = 1;
        }
        else if(cc.X == stpos.X + w -1 && cc.Y == stpos.Y + h - 1) 
        {
            pchar(box->br);
            axs = 0;
            dir = -1;
        }
        else if(axs == 0) pchar(box->h);
        else pchar(box->v);
        
        if(axs == 0) cc.X+=dir;
        else cc.Y+=dir;
    } while (cc.X != stpos.X || cc.Y != stpos.Y);
    gotoxy(stpos);
    ++stpos.X; ++stpos.Y;
    gotoxy(stpos);
}

FormData* form(char** labels, int n, COORD pos)
{
    
}

void freeRoam()
{
    char x = 0;
    while (x != '=')
    {
        x = _getch();
        COORD cord = wherexy();
        if (x == '`')
            printf("%d,%d", cord.X, cord.Y);
        else if (x != '=')
        {
            x = _getch();
            if (x == 72)
                cord.Y -= 1;
            else if (x == 77)
                cord.X += 1;
            else if (x == 80)
                cord.Y += 1;
            else if (x == 75)
                cord.X -= 1;
            gotoxy(cord);
        }
    }
}

void xinput(char *s)
{
    char c = ' ';
    while(1)
    {
        c = _getch();
        if(c==13) break;
        *s = c;
        _putch(c);
        s++;
    }
    *s = '\0'; 
}