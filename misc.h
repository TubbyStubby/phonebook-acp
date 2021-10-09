/*
    Author: TubbyStubby

    functions:
        gotoxy
        wherex
        wherey
        mdscr       (use only once in the start as it clears the screen)
        midprint    ()
        arrowSelect (only vertical use. Need to pass position of first item and total number of items)
        freeRoam    (press ` at any point to print co-ord's, = to exit)
        menu

    Note: currently this works for windows only

    Ex - Menu
    int main()
    {
        int mOS = mdscr();
        midprint("Main Menu\n");
        newMenuLine "1. First";
        newMenuLine "2. Sec";
        newMenuLine "3. Third";
        newMenuLine "4. Fourth";
        int choice = arrowSelect(mOS-8,wherey()-3,4);
        cout<<"\n"<<choice;
        _getch();
    }
*/
#include<windows.h>
#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<string>
#define newMenuLine gotoxy(mOS-8,wherey()+1);cout<<
#define updatearrw gotoxy(x-2,y);cout<<'>';gotoxy(x-2,y);

using namespace std;

void welcome(void);
void gotoxy(int,int);
int wherex(void);
int wherey(void);
int mdscr(void);
void midprint(char*);
int arrowSelect(int,int,int);
void freeRoam(void);
int menu(char [][20]);

int mOS = mdscr();

void gotoxy(int x,int y)
{
    COORD cord;
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbiInfo);
    return csbiInfo.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbiInfo);
    return csbiInfo.dwCursorPosition.Y;
}

int mdscr()
{
    int x;
    for(x=0;wherey()!=1;x++)
        cout<<' ';
    system("cls");
    return x%2!=0?(x+1)/2:x/2;
}

void midprint(char *a)
{
    gotoxy(abs(mOS-strlen(a)/2),wherey());
    cout<<a;
}

int arrowSelect(int x,int y,int n)
{
    int ch=1,oy=y;
    char arr;
    gotoxy(x-2,y);
    cout<<'>';
    gotoxy(x-2,y);
    while(true)
    {
        arr=_getch();
        if(arr==13)
            break;
        else if(arr==-32)
        {
            arr=_getch();
            if(arr==72)
            {
                cout<<' ';
                if(ch==1)
                {
                    ch=n;
                    y=y+n-1;
                    updatearrw
                }
                else
                {
                    ch--;
                    y=y-1;
                    updatearrw
                }
            }
            else if(arr==80)
            {
                cout<<' ';
                if(ch==n)
                {
                    ch=1;
                    y=y-n+1;
                    updatearrw
                }
                else
                {
                    ch++;
                    y=y+1;
                    updatearrw
                }
            }
        }
    }
    gotoxy(0,oy+n);
    return ch;
}

void freeRoam()
{
    char x=0;
    while(x!='=')
    {
        x=_getch();
        if(x=='`')
            cout<<wherex()<<','<<wherey();
        else if(x!='=')
        {
            x=_getch();
            if(x==72)
                gotoxy(wherex(),wherey()-1);
            else if(x==77)
                gotoxy(wherex()+1,wherey());
            else if(x==80)
                gotoxy(wherex(),wherey()+1);
            else if(x==75)
                gotoxy(wherex()-1,wherey());
        }
    }
}

int menu(char items[][20])
{
    system("cls");
    int i=0,ch;
    cout<<'\n';
    midprint("---MAIN MENU---");
    cout<<endl;
    int inx=mOS-5,iny=wherey()+1;
    for(i=0;items[i][0]!='\0';i++)
    {
        gotoxy(mOS-5,wherey()+1);cout<<items[i];
    }
    gotoxy(mOS-5,wherey()+1);cout<<"Exit\n\n";
    ch=arrowSelect(inx,iny,i+1);
    if(ch==i+1)
        exit(0);
    else
        return ch;
}

void welcome()
{
    for(int i=0;i<151;i+=15)
    {
        gotoxy(mOS-6,10);
        cout<<"--Welcome User--";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),i);
        Sleep(100);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    _getch();
}
