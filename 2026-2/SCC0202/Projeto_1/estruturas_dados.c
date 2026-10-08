#include <stdbool.h>
#include "linked_list.h"
#include "sequencial_list.h"

typedef struct fila
{
    LINKED_LIST (*create_linked_list)(void);
    bool (*delete_linked_list)(LINKED_LIST **linked_list);
    bool (*add_end)(LINKED_LIST *linked_list, int data);
    bool (*remove_start)(LINKED_LIST *linked_list);
    //Get First
}FILA;

typedef struct pilha
{
    LINKED_LIST (*create_linked_list)(void);
    bool (*delete_linked_list) (LINKED_LIST **linked_list);
    bool (*add_start)(LINKED_LIST *linked_list, int data);
    bool (*remove_start)(LINKED_LIST *linked_list);
    //Get First
}PILHA;

typedef struct lista_sequencial
{
    SEQUENCIAL_LIST (*create_sequencial_list)(void);
    bool (*delete_sequencial_list)(SEQUENCIAL_LIST **sequencial_list);
    bool (*sl_add_start)(SEQUENCIAL_LIST *sequencial_list, int data);
    bool (*st_add_end)(SEQUENCIAL_LIST *sequencial_list, int data);
    bool (*sl_remove_start)(SEQUENCIAL_LIST *sequencial_list);
    bool (*sl_remove_end)(SEQUENCIAL_LIST *sequencial_list);
}LISTA_SEQUENCIAL;

typedef struct lista_encadeada
{
    LINKED_LIST (*create_linked_list)(void);
    bool (*delete_linked_list)(LINKED_LIST **linked_list);
    bool (*add_start)(LINKED_LIST *linked_list, int data);
    bool (*add_end)(LINKED_LIST *linked_list, int data);
    bool (*remove_start)(LINKED_LIST *linked_list);
    bool (*remove_end)(LINKED_LIST *linked_list);
}LISTA_ENCADEADA;

typedef struct lista_ordenada
{
    SEQUENCIAL_LIST (*create_sequencial_list)(void);
    bool (*delete_sequencial_list)(SEQUENCIAL_LIST **sequencial_list);
    bool (*add_ordering)(SEQUENCIAL_LIST *sequencial_list, int data);
    bool (*sl_remove_start)(SEQUENCIAL_LIST *sequencial_list);
    bool (*sl_remove_end)(SEQUENCIAL_LIST *sequencial_list);
    bool (*binary_search)(SEQUENCIAL_LIST *sequencial_list, int target);
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
