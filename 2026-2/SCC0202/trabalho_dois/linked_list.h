#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node NODE;

extern NODE *newitem(int value);
extern NODE *addfront(NODE *listp, NODE *newp);
extern NODE *addend(NODE *listp, NODE *newp);
extern NODE *delitem(NODE *listp, int value);
extern NODE *lookup(NODE *listp, int value);
extern void apply(NODE *listp, void(*fn)(NODE*, void*), void *arg);
extern void freall(NODE *listp);

#endif
