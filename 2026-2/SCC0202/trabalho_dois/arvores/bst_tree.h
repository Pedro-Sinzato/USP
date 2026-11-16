#ifndef BST_TREE_H
#define BST_TREE_H

typedef struct node NODE;

/* Modo de inserção. Use sempre o MESMO modo para todas as inserções de uma árvore. */
typedef enum { BST, AVL, LLRB } TREE_MODE;

extern NODE *newitem(int value);
extern NODE *insert(NODE *treep, NODE *newp, TREE_MODE mode);
extern NODE *lookup(NODE *treep, int value);
extern NODE *nrlookup(NODE *treep, int value);

extern void applypreorder(NODE *treep, void (*fn)(NODE*, void*), void *arg);
extern void applyinorder(NODE *treep, void (*fn)(NODE*, void*), void *arg);
extern void applypostorder(NODE *treep, void (*fn)(NODE*, void*), void *arg);

#endif
