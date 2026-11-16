#ifndef LLRB_H
#define LLRB_H

#include "bst_node.h"

/* Prepara um nó recém-criado para uso numa LLRB (nasce vermelho). */
void llrb_init(NODE *p);

/* Se os dois filhos de h são vermelhos, inverte as cores (h, left, right).
 * Chamada descendo, antes de comparar (variante 2-3-4 do artigo). */
void llrb_split(NODE *h);

/* Corrige ligações vermelhas à direita ou duas vermelhas seguidas à esquerda.
 * Chamada na volta da recursão; devolve a nova raiz da subárvore. */
NODE *llrb_fixup(NODE *h);

/* A raiz da árvore é sempre preta. */
NODE *llrb_blacken(NODE *root);

#endif
