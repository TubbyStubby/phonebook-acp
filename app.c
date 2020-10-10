#include "cmisc.h"

int main() 
{
    COORD wSize = getWindowSize(), zz;
    zz.X = zz.Y = 0;
    drawBox(&dlBox, wSize.X, wSize.Y, zz);
    char s[20];
    xinput(s);
    _getch();
}