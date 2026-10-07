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

typedef struct lista_sequencial
{

}LISTA_SEQUENCIAL;

typedef struct lista_encadeada
{

}LISTA_ENCADEADA;

typedef struct lista_ordenada
{

}LISTA_ORDENADA;

typedef union estruras_dados
{
    FILA fila;
    PILHA pilha;
    LISTA_SEQUENCIAL lista_sequencial;
    LISTA_ENCADEADA lista_encadeada;
    LISTA_ORDENADA lista_ordenada;
}ESTRURAS_DADOS;
//To safely select between structs, you must include a discriminator (often an int or enum) outside
// the union or as the first member of all contained structs to track which struct is currently active.
// Accessing a member involves checking this discriminator first, then accessing
// the corresponding struct field, as writing to one member overwrites the others due to shared memory. (Verificar)
