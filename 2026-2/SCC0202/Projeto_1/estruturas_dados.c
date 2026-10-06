#include <stdbool.h>
#include "linked_list.h"

typedef struct fila
{
    LINKED_LIST (*create_linked_list)(void);
    //Deletar LINKED_LIST
    bool (*add_end)(LINKED_LIST *linked_list, int data);
    bool (*remove_end)(LINKED_LIST *linked_list);
}FILA;

typedef struct pilha
{
    LINKED_LIST (*create_linked_list)(void);
    //Deletar LINKED_LIST
    bool (*add_start)(LINKED_LIST *linked_list, int data);
    bool (*remove_start)(LINKED_LIST *linked_list);
}PILHA;

typedef struct lista_encadeada
{

}LISTA_ENCADEADA;

typedef union estruras_dados
{
    FILA fila;
    PILHA pilha;
}ESTRURAS_DADOS;
