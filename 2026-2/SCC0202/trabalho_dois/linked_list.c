#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct node
{
    int value;
    NODE *next;
};

NODE *newitem(int value)
{
    NODE *newp;

    newp = calloc(1, sizeof(NODE));
    newp->value = value;
    newp->next = NULL;
    return newp;
}

NODE *addfront(NODE *listp, NODE *newp)
{
    newp->next = listp;
    return newp;
}
NODE *addend(NODE *listp, NODE *newp)
{
    NODE *p;

    if(listp == NULL)
    {
        return newp;
    }
    for(p = listp; p->next != NULL; p = p->next);
    p->next = newp;
    return listp;
}
NODE *delitem(NODE *listp, int value)
{
    NODE *p, *prev;

    prev = NULL;
    for(p = listp; p != NULL; p = p->next)
    {
        if(value == p->value)
        {
            if(prev == NULL)
            {
                listp = p->next;
            }
            else
            {
                prev->next = prev->next;
            }
            free(p);
            return listp;
        }
        prev = p;
    }
    return NULL;
}

NODE *lookup(NODE *listp, int value)
{
    for(; listp != NULL; listp = listp->next)
    {
        if(value == listp->value)
        {
            return listp;
        }
    }

    return NULL;
}

void apply(NODE *listp, void(*fn)(NODE*, void*), void *arg)
{
    for(; listp != NULL; listp = listp->next)
    {
        (*fn)(listp, arg);
    }
}
void printv(NODE *p, void *arg)
{
    char *fmt;
    fmt = (char *) arg;
    printf(fmt, p->value);
}
void inccounter(NODE *p, void *arg)
{
    int *ip;
    ip = (int *) arg;
    (* ip)++;
}
void freeall(NODE *listp)
{
    NODE *next;
    for(; listp != NULL; listp = next)
    {
        next = listp->next;
        free(listp);
    }
}
