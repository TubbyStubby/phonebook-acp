#include "cmisc.h"
#define arrow(x, y) gotoxy(_cord(x,y));putch('>');gotoxy(_cord(x,y))

typedef struct
{
    char *name;
    char *address;
    char *mobile;
} Contact;

typedef struct TrieNode
{
    char val;
    Contact *ct;
    struct TrieNode *next[27];
    struct TrieNode *parent;
} TNode;

short addContact(char**, int, TNode*,COORD);
short listContacts(TNode*);

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
    // char *l[] = {"Hello", "World", "2020"};
    // COORD fp = {1,1};
    // FormData *f = form(l, 3, fp);

    //---Menu
    char *mItems[] = { "Add", "List", "Modify", "Delete", "Search" };
    char *labels[] = {"Name", "Address", "Mobile"};

    short n = 5;
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
            
            default:
                break;
            }
        }
        
        arrow(2,5+mi);
    }
}

short addContact(char **l, int n, TNode *root, COORD cord)
{
    FormData *f;
    f = form(l,n,cord);

    TNode *t = root;
    char *s = f->data.s;

    while(*s != '\0')
    {
        if(t->next[*s - 'a'] == NULL)
        {
            TNode *new = malloc(sizeof(TNode));
            new->ct = NULL;
            for(int i=0; i<27; i++) new->next[i] = NULL;
            new->parent = t;
            new->val = *s;
            t->next[*s - 'a'] = new;
        }
        else
        {
            t = t->next[*s - 'a'];
            s++;
        }
    }

    if(t->ct == NULL)
    {   
        Contact *ct = malloc(sizeof(Contact));
        ct->address = malloc(sizeof(char)*20);
        ct->name = malloc(sizeof(char)*20);
        ct->mobile = malloc(sizeof(char)*20);
        strcpy(ct->address, f[1].data.s);
        strcpy(ct->name, f[0].data.s);
        strcpy(ct->mobile, f[2].data.s);

        t->ct = ct;

        return 0;
    }
    else return 1;    
}
