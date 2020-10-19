#ifndef PHONEBOOK_H
#define PHONEBOOK_H

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

#endif