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

void clearBox(int w, int h, COORD stpos)
{
    gotoxy(stpos);
    COORD cc = stpos;
    short dir=1, axs=0;
    do
    {
        gotoxy(cc);
        
        if(cc.X == stpos.X + w - 1 && cc.Y == stpos.Y) axs = 1;
        else if(cc.X == stpos.X && cc.Y == stpos.Y + h -1) axs = 1;
        else if(cc.X == stpos.X + w -1 && cc.Y == stpos.Y + h - 1) 
        {
            axs = 0;
            dir = -1;
        }

        pchar(' ');
        
        if(axs == 0) cc.X+=dir;
        else cc.Y+=dir;
    } while (cc.X != stpos.X || cc.Y != stpos.Y);
}

FormData* form(char** labels, int n, COORD pos)
{
    short labelW = 15, inputW = 15;
    
    drawBox(&slBox, 33, n*2+3, pos);
    for(int i = 0; i < n; ++i)
    {
        gotoxy(_cord(pos.X + 1, pos.Y + 2*(i+1)));
        printf(*labels);
        labels++;
    }
    vLine(_cord(pos.X+10, pos.Y+1), n*2+1);

    FormData *f = (FormData*)malloc(sizeof(FormData)*n);

    for(int i=0; i<n; ++i)
    {
        gotoxy(_cord(pos.X+11, pos.Y+1+i*2));
        drawBox(&slBox, 15, 3, wherexy());
        f[i].t = str;
        xinput(f[i].data.s);
        clearBox(15, 3, _cord(pos.X+11, pos.Y+1+i*2));
    }

    gotoxy(pos);
    for(int i=0; i<n*2+3; ++i) {
        for(int j=0; j<33; ++j) {
            gotoxy(_cord(pos.X + j, pos.Y + i));
            pchar(' ');
        }
    }

    return f;
}

void vLine(COORD pos, int sz)
{
    for(int i=0; i<sz; ++i)
    {
        gotoxy(_cord(pos.X, pos.Y+i));
        pchar(slBox.v);
    }
}

void hLine(COORD pos, int sz)
{
    for(int i=0; i<sz; ++i)
    {
        gotoxy(_cord(pos.X+i, pos.Y));
        pchar(slBox.h);
    }
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
        else if(c!=8)
        {    
            *s = c;
            _putch(c);
            s++;
        }
        else
        {
            s--;
            _putch(' ');
            _putch(8);
        }    
    }
    *s = '\0'; 
}