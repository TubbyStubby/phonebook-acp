#include "phonebook.h"

short addContact(char **l, int n, TNode *root, COORD cord)
{
    FormData *f = NULL;
    if(n!=-1) 
    {
        f = form(l,n,cord);
        if(f==NULL) return 2; //canceled
    }

    TNode *t = root;
    char *s = n==-1 ? l[0] : f->data.s;

    while(*s != '\0')
    {
        int spcchk;
        if(*s == ' ') spcchk = 26;
        else spcchk = *s - 'a';
        if(t->next[spcchk] == NULL)
        {
            TNode *new = malloc(sizeof(TNode));
            new->ct = NULL;
            for(int i=0; i<27; i++) new->next[i] = NULL;
            new->parent = t;
            new->val = *s;
            t->next[spcchk] = new;
        }
        else
        {
            t = t->next[spcchk];
            s++;
        }
    }

    if(t->ct == NULL)
    {   
        Contact *ct = malloc(sizeof(Contact));
        ct->address = malloc(sizeof(char)*20);
        ct->name = malloc(sizeof(char)*20);
        ct->mobile = malloc(sizeof(char)*20);
        strcpy(ct->address, n==-1?l[1]:f[1].data.s);
        strcpy(ct->name, n==-1?l[0]:f[0].data.s);
        strcpy(ct->mobile, n==-1?l[2]:f[2].data.s);

        t->ct = ct;

        if(f!=NULL) saveToFile(ct, SAVE_FILE);

        return 0;
    }
    else return 1;    
}

Contact* search(TNode *root, void (*modifier)(Contact**))
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
            if(c==27) break; //esc key
            _putch(c);
            if(c!=8) //8 is backspace
            {
                last_c = wherexy();
                int spcchk;
                if(c==' ') spcchk = 26;
                else spcchk = c-'a';
                root = root->next[spcchk] != NULL ? root->next[spcchk] : root;
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
                    printf("%s", (*(ctmp->c))->name);
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
                        if(modifier==NULL) 
                        {
                            Contact *tct = *(ctmp->c);
                            freeCL(cl);
                            return tct;
                        }
                        else
                        {
                            gotoxy(_cord(pos.X + w, pos.Y));
                            modifier(ctmp->c);
                            freeCL(ctmp);
                            break;
                        }
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
    clearArea(pos);
    return NULL;
}

CLink* trieWalk(TNode *root)
{
    if(root == NULL) return NULL;
    CLink *head=NULL;
    if(root->ct != NULL) {
        head = malloc(sizeof(CLink));
        head->c = &(root->ct);
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

void deleteContact(Contact **ct)
{
    deleteFileRecord(*ct, SAVE_FILE);
    COORD pos = wherexy();
    gotoxy(pos);
    char *a[] = {"Confirm", "Cancel"};
    int x = aroSelect(a, 2);
    clearArea(pos);
    Contact *dref = *ct;
    switch (x)
    {
    case 0:
        free(dref);
        *ct = NULL;
        break;
    }
}

void modifyContact(Contact **ct)
{
    char oldName[20];
    strcpy(oldName, (*ct)->name);
    char *lbls[] = {"Name", "Address", "Mobile"};
    COORD pos = wherexy();
    FormData *f = form(lbls, 3, pos);
    gotoxy(pos);
    char *a[] = {"Confirm", "Cancel"};
    int x = aroSelect(a, 2);
    clearArea(pos);
    switch (x)
    {
    case 0:
        if(f->data.s[0] != '\0') strcpy((*ct)->name, f->data.s);
        f++;
        if(f->data.s[0] != '\0') strcpy((*ct)->address, f->data.s);
        f++;
        if(f->data.s[0] != '\0') strcpy((*ct)->mobile, f->data.s);
        updateFileRecord(oldName,*ct, SAVE_FILE);
        break;
    }
}

int saveToFile(Contact *ct, char *fname)
{
    FILE *fo = fopen(fname, "a");
    if(fo == NULL) return 1;
    else
    {
        fprintf(fo, "%s\n%s\n%s\n", ct->name, ct->address, ct->mobile);
        fclose(fo);
    }
    return 0;
}

int updateFileRecord(char *oldName, Contact *ct, char *fname)
{
    FILE *fw = fopen("temp.txt", "w");
    FILE *fr = fopen(fname, "r");
    if(fr == NULL || fw == NULL) return 1;
    else
    {
        char str[20];
        while(1)
        {
            long cpos = ftell(fr);
            if(fscanf(fr, "%s", str) == EOF) break;
            if(strcmp(str, oldName))
            {
                fseek(fr, cpos, SEEK_SET);
                for(int i=0; i<3; i++)
                {
                    fscanf(fr, "%s", str);
                    fputs(str, fw);
                    fputc('\n', fw);
                }
            }
            else
            {
                fscanf(fr, "%s", str);
                fscanf(fr, "%s", str);
                fputs(ct->name, fw); fputc('\n', fw);
                fputs(ct->address, fw); fputc('\n', fw);
                fputs(ct->mobile, fw); fputc('\n', fw);
            }
        }
    }
    fclose(fr);
    fclose(fw);
    remove(fname);
    rename("temp.txt", fname);
    return 0;
}

int deleteFileRecord(Contact *ct, char *fname)
{
    FILE *fw = fopen("temp.txt", "w");
    FILE *fr = fopen(fname, "r");
    if(fr == NULL || fw == NULL) return 1;
    else
    {
        char str[20];
        while(1)
        {
            long cpos = ftell(fr);
            if(fscanf(fr, "%s", str) == EOF) break;
            if(strcmp(str, ct->name))
            {
                fseek(fr, cpos, SEEK_SET);
                for(int i=0; i<3; i++)
                {
                    fscanf(fr, "%s", str);
                    fputs(str, fw);fputc('\n', fw);
                }
            }
            else
            {
                fscanf(fr, "%s", str);
                fscanf(fr, "%s", str);
            }
        }
    }
    fclose(fr);
    fclose(fw);
    remove(fname);
    rename("temp.txt", fname);
    return 0;
}

int loadContacts(TNode *root, char *fname)
{
    FILE *fr = fopen(fname, "r");
    if(fr == NULL) return 1;
    char *str[3];
    for(int i=0; i<3; i++) str[i] = (char*)malloc(sizeof(char)*20);
    while(1)
    {
        int f = 0;
        for(int i=0;i<3;i++) 
        {
            if(fscanf(fr, "%s", str[i]) == EOF)
            {  
                f=1;
                break;
            }
        }
        if(f==0) addContact(str, -1, root, _cord(-1,-1));
        else break;
    }
    fclose(fr);
    return 0;
}