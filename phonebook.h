#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "cmisc.h"
#include "stdlib.h"

#define SAVE_FILE "contacts.txt"

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
    Contact **c;
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
Contact* search(TNode*, void (*modifier)(Contact**));

//returns circular ll
CLink* trieWalk(TNode*);

void freeCL(CLink*);

//delete
void deleteContact(Contact**);

//modify
void modifyContact(Contact**);

//append the contact in the file
int saveToFile(Contact*, char*);

//to update, with, in file
int updateFileRecord(char*, Contact*, char*);

//to delete in file
int deleteFileRecord(Contact*, char*);

//trie root, filename
int loadContacts(TNode*, char*);

#endif