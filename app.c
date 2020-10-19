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

typedef struct CLink
{
    Contact *c;
    struct CLink *next;
    struct CLink *prev;
} CLink;

typedef struct TrieNode
{
    char val;
    Contact *ct;
    struct TrieNode *next[27];
    struct TrieNode *parent;
} TNode;

short addContact(char**, int, TNode*,COORD);

//searches and reutrns a contact
Contact* search(TNode*);

//returns circular ll
CLink* trieWalk(TNode*);

void freeCL(CLink*);

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
                Contact *ct = search(&root);
                if(ct != NULL) printf("%s, %s, %s", ct->name, ct->address, ct->mobile);
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
    short w=30, h=15;
    COORD pos = wherexy();
    drawBox(&slBox, w, h, pos);
    drawBox(&slBox, w-2, 3, wherexy());
    COORD in_cord = wherexy();
    drawBox(&slBox, w-2, h-5, _cord(pos.X+1, pos.Y+4));
    COORD out_cord = wherexy();

    gotoxy(in_cord);

    char c;
    short x = 0, windo = 5; //for display
    CLink *cl = NULL;
    while (1)
    {
        c = _getch();
        COORD last_c;
        if(c!=-32) //arrow key not pressed
        { 
            if(c==27) break;
            _putch(c);
            if(c!=8) //8 is backspace
            {
                last_c = wherexy();
                root = root->next[c-'a'] != NULL ? root->next[c-'a'] : root;
            }
            else
            {
                _putch(' ');
                _putch(8);
                last_c = wherexy();
                root = root->parent != NULL ? root->parent : root;
            }
            
            //clear output area
            for(int i=0; i<h-7; ++i)
            {
                gotoxy(_cord(out_cord.X, out_cord.Y+i));
                for(int j=0; j<w-4; ++j) _putch(' ');
            }

            //walk and display
            freeCL(cl);
            x=0;
            cl = trieWalk(root);
            if(cl!=NULL)
            {
                CLink *ctmp = cl;
                do 
                {
                    gotoxy(_cord(out_cord.X+1, out_cord.Y+(x++)));
                    printf("%s", ctmp->c->name);
                    ctmp = ctmp->next;
                }while(ctmp != cl && x<windo);
            }
            else
            {
                //clear output area
                for(int i=0; i<h-7; ++i)
                {
                    gotoxy(_cord(out_cord.X, out_cord.Y+i));
                    for(int j=0; j<w-4; ++j) _putch(' ');
                }
            }
        }
        else //arrow key pressed
        {
            if(cl!=NULL)
            {
                CLink *ctmp = cl;
                int chc=0;
                arrow(out_cord.X, out_cord.Y+chc);
                while(1)
                {
                    char aro = _getch();
                    if(aro == 72 && chc-1>=0) //up arrow
                    {
                        chc--;
                        ctmp = ctmp->prev;
                    }
                    else if(aro == 80 && chc+1<x) //down arrow
                    {
                        chc = chc + 1;
                        ctmp = ctmp->next;
                    }
                    _putch(' ');
                    arrow(out_cord.X, out_cord.Y+chc);
                    aro = _getch();
                    if(aro == -32) continue;
                    else if(aro == 13)
                    {
                        Contact *tct = ctmp->c;
                        freeCL(cl);
                        return tct;
                    }
                    else if(aro == 27) break;
                }
            }
            else _getch(); //remove arrow from buffer
        }
        
        //back to input box
        gotoxy(last_c);
    }

    //cleaning CLink space
    freeCL(cl);
    return NULL;
}

CLink* trieWalk(TNode *root)
{
    if(root == NULL) return NULL;
    CLink *head=NULL;
    if(root->ct != NULL) {
        head = malloc(sizeof(CLink));
        head->c = root->ct;
        head->next = head;
        head->prev = head;
    }
    for(int i=0; i<27; ++i)
    {
        CLink *t = trieWalk(root->next[i]);
        if(t!=NULL)
        {
            if(head == NULL) head = t;
            else
            {
                head->prev->next = t;
                t->prev->next = head;
                CLink *ctmp = head->prev;
                head->prev = t->prev;
                t->prev=ctmp;
            }
        }
    }
    return head;
}

void freeCL(CLink *cl)
{
    if(cl == NULL) return;
    CLink *ctmp = cl;
    do 
    {
        CLink *nxt = ctmp->next;
        free(ctmp);
        ctmp = nxt;
    }while(ctmp != cl);
}