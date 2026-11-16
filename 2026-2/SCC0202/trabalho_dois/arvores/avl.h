#ifndef AVL_H
#define AVL_H

#include "bst_node.h"

/* Prepara um nó recém-criado para uso numa AVL (altura 1). */
void avl_init(NODE *p);

/* Atualiza a altura de h e aplica as rotações necessárias.
 * Deve ser chamada na volta da recursão; devolve a nova raiz da subárvore. */
NODE *avl_rebalance(NODE *h);

#endif
