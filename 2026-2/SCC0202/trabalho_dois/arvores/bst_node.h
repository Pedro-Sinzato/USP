#ifndef BST_NODE_H
#define BST_NODE_H

#include "bst_tree.h" /* typedef struct node NODE */

enum { BLACK = 0, RED = 1 };

struct node
{
    int value;
    NODE *left;  /* lesser */
    NODE *right; /* greater */

     /*
     *   AVL  -> info.height (folha = 1, NULL = 0)
     *   LLRB -> info.color  (RED ou BLACK)
     *   BST  -> não usa
     */
    union
    {
        int height;
        int color;
    } info;
};

#endif
