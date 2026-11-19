#include <stdio.h>
#include <stdlib.h>
#include "bst_tree.h"
#include "avl.h"
#include "llrb.h"

//Protótipos, criar funções depois.
int int_insert(NODE* treep, NODE *newp);
int double_insert(NODE* treep, NODE *newp);
int char_insert(NODE* treep, NODE *newp);
int str_insert(NODE* treep, NODE *newp);
int bool_insert(NODE* treep, NODE *newp);

NODE *newitem(int value)
{
    NODE *newp;

    newp = calloc(1, sizeof(NODE));
    if(newp == NULL)
    {
        return NULL;
    }
    newp->value = value;
    newp->left = NULL;
    newp->right = NULL;

    return newp;
}

/* ---------- Inserção 1: BST simples ---------- */
static NODE *bst_insert(NODE *treep, NODE *newp,  int (*fn)(NODE*, NODE*))
{
    if(treep == NULL)
    {
        return newp;
    }
    if(newp->value == treep->value)
    {
        free(newp);
        return treep;
    }
    else if((*fn)(treep, newp))
    {
        treep->left = bst_insert(treep->left, newp, *fn);
    }
    else
    {
        treep->right = bst_insert(treep->right, newp, *fn);
    }
    return treep;
}

/* ---------- Inserção 2: AVL (balanceamento em avl.c) ---------- */
static NODE *avl_insert(NODE *treep, NODE *newp, int (*fn)(NODE*, NODE*))
{
    if(treep == NULL)
    {
        avl_init(newp);
        return newp;
    }
    if(newp->value == treep->value)
    {
        free(newp);
        return treep;
    }
    else if((*fn)(treep, newp))
    {
        treep->left = avl_insert(treep->left, newp, *fn);
    }
    else
    {
        treep->right = avl_insert(treep->right, newp, *fn);
    }
    return avl_rebalance(treep);
}

/* ---------- Inserção 3: LLRB (balanceamento em llrb.c) ---------- */
static NODE *llrb_insert(NODE *treep, NODE *newp, int (*fn)(NODE*, NODE*))
{
    if(treep == NULL)
    {
        llrb_init(newp);
        return newp;
    }

    llrb_split(treep);

    if(newp->value == treep->value)
    {
        free(newp);
        return treep;
    }
    else if((*fn)(treep, newp))
    {
        treep->left = llrb_insert(treep->left, newp, *fn);
    }
    else
    {
        treep->right = llrb_insert(treep->right, newp, *fn);
    }
    return llrb_fixup(treep);
}

NODE *insert(NODE *treep, NODE *newp, TREE_MODE mode, int type)
{
    int (*fn)(NODE*, NODE*);
    switch (type)
    {
        case INT:
            fn = int_insert;
            break;
        case DOUBLE:
            fn = double_insert;
            break;
        case STRING:
            fn = str_insert;
            break;
        case CHAR:
            fn = char_insert;
            break;
        case BOOL:
            fn = bool_insert;
    }

    switch(mode)
    {
        case AVL:
            return avl_insert(treep, newp, fn);
        case LLRB:
            return llrb_blacken(llrb_insert(treep, newp, fn));
        case BST:
        default:
            return bst_insert(treep, newp, fn);
    }
}

NODE *lookup(NODE *treep, int value)
{
    if(treep == NULL)
    {
        return NULL;
    }

    if(value == treep->value)
    {
        return treep;
    }
    else if(value < treep->value)
    {
        return lookup(treep->left, value);
    }
    else
    {
        return lookup(treep->right, value);
    }
}

NODE *nrlookup(NODE *treep, int value)
{
    while(treep != NULL)
    {
        if(value == treep->value)
        {
            return treep;
        }
        else if(value < treep->value)
        {
            treep = treep->left;
        }
        else
        {
            treep = treep->right;
        }
    }
    return NULL;
}

void applypreorder(NODE *treep, void (*fn)(NODE*, void*), void *arg)
{
    if(treep == NULL)
    {
        return;
    }
    (*fn)(treep, arg);
    applypreorder(treep->left, *fn, arg);
    applypreorder(treep->right, *fn, arg);
}

void applyinorder(NODE *treep, void (*fn)(NODE*, void*), void *arg)
{
    if(treep == NULL)
    {
        return;
    }
    applyinorder(treep->left, *fn, arg);
    (*fn)(treep, arg);
    applyinorder(treep->right, *fn, arg);
}

void applypostorder(NODE *treep, void(*fn)(NODE*, void*), void *arg)
{
    if(treep == NULL)
    {
        return;
    }
    applypostorder(treep->left, *fn, arg);
    applypostorder(treep->right, *fn, arg);
    (*fn)(treep, arg);
}
