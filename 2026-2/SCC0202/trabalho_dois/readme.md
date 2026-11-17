### Como usar o arquivo bst_tree.c

#### Criando árvore e inserindo na árvore:

NODE *root = insert(root, newitem(value), mode)

Mode: BST, AVL e LLRB

#### Buscando na árvore

NODE *target = lookup(root, value);
NODE *target = nrlookup(root, value);

#### Funções de apply

Executam uma função para todos os nós da árvore, exemplo:

void print_node(NODE *node)
{
  printf("%d\n", node->value);
}

extern void applypreorder(root, print_node, NULL);

char *message = "Nó apagado";
void delete_tree(NODE *node, char *message)
{
  free(node);
  printf("%s\n", message);
}
extern void applypostorder(root, delete_tree, message);
