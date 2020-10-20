#include "phonebook.h"

int main()
{
    TNode root;
    root.val = '.';
    root.ct = NULL;
    for(int i=0; i<27; i++) root.next[i] = NULL;
    root.parent = NULL;

    COORD wSize = getWindowSize(), zz;
    zz.X = zz.Y = 0;
    drawBox(&dlBox, wSize.X, wSize.Y, zz);

    //---Menu
    char *mItems[] = { "Add", "Modify", "Delete", "Search" };
    char *labels[] = {"Name", "Address", "Mobile"};

    short n = 4;
    drawBox(&slBox, 11, n+6, _cord(1,1));
    printf("Menu");
    hLine(_cord(2,3), 9);
    for(int i=0; i<n; ++i)
    {
        gotoxy(_cord(3,5+i));
        printf(mItems[i]);
    }
    char ch = ' ';
    short mi = 0;
    arrow(2,5);
    while(1)
    {
        ch = _getch();

        if(ch==-32) //arrow key pressed
        {
            ch = _getch(); //get the arrow key
            if(ch == 72) mi = mi - 1 < 0 ? n-1 : mi - 1; //up arrow
            else if(ch == 80) mi = (mi + 1) % n; //down arrow
            putch(' ');
        }
        else if(ch == 13)
        {
            switch (mi)
            {
            case 0:
                addContact(labels, 3, &root, _cord(12, 1));
                break;
            case 2:
                gotoxy(_cord(12,1));
                search(&root, deleteContact);
                break;
            case 3:
                gotoxy(_cord(12,1));
                Contact *ct = search(&root, NULL);
                if(ct != NULL) printf("%s, %s, %s", ct->name, ct->address, ct->mobile);
                break;
            default:
                break;
            }
        }
        
        arrow(2,5+mi);
    }
}
