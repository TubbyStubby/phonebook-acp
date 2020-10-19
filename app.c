#include "cmisc.h"
#define arrow(x, y) gotoxy(_cord(x,y));putch('>');gotoxy(_cord(x,y))

typedef struct
{
    char *name;
    char *address;
    char *mobile;
    struct Contact *next;
    struct Contact *prev;
} Contact;

typedef struct
{
    Contact *c;
    DLL link;
} CLink;

typedef struct TrieNode
{
    char val;
    Contact *ct;
    struct TrieNode *next[27];
    struct TrieNode *parent;
} TNode;

short addContact(char**, int, TNode*,COORD);
Contact* search(TNode*);
int getN(int, TNode*, COORD);

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
            case 4:
                gotoxy(_cord(12,1));
                search(&root);
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

Contact* search(TNode *root)
{
    short w=40, h=20;
    COORD pos = wherexy();
    drawBox(&slBox, w, h, pos);
    drawBox(&slBox, w-2, 3, wherexy());
    drawBox(&slBox, w-2, h-5, _cord(pos.X+1, pos.Y+4));

    gotoxy(_cord(pos.X+3, pos.Y+2));

    char c;
    short x = 0;
    while (1)
    {
        c = _getch();
        COORD tco;
        _putch(c);
        if(c!=8) //8 is backspace
        {
            ++x;
            
            tco = wherexy();
            if(root!=NULL) root = root->next[c-'a'];
        }
        else
        {
            --x;
            _putch(' ');
            _putch(8);
            tco = wherexy();
            root = root->parent;
        }
        for(int i=0; i<h-7; ++i)
        {
            gotoxy(_cord(pos.X+2, pos.Y+5+i));
            for(int j=0; j<w-4; ++j) _putch(' ');
        }
        int x = getN(4, root, _cord(pos.X+2, pos.Y+5));
        gotoxy(tco);
    }
    
}

int getN(int n, TNode *root, COORD pos)
{
    if(root == NULL) return n;
    gotoxy(pos);
    if(root->ct != NULL) {
        printf("%s", root->ct->name);
        gotoxy(_cord(pos.X, pos.Y+1));
        --n;
    }
    if(n==0) return n;
    for(int i=0; i<27; ++i)
    {
        n = getN(n, root->next[i], wherexy());
        if(n == 0) break;
    }
    return n;
}