#ifndef BST_NODE_H
#define BST_NODE_H

/*
 * Definição PRIVADA do nó: só bst_tree.c, avl.c e llrb.c incluem este arquivo.
 * Quem usa a árvore enxerga NODE apenas como tipo opaco (bst_tree.h).
 */

#include "bst_tree.h" /* typedef struct node NODE */

enum { BLACK = 0, RED = 1 };

struct node
{
    int value;
    NODE *left;  /* lesser */
    NODE *right; /* greater */

    /* Uma árvore usa um único modo, então o mesmo espaço serve para os dois:
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
