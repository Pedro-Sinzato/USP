#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../item.h"
#include "ordered_list.h"
#include "../estruturas_dados.h"

struct node
{
    ITEM *item;
    NODE *previous;
    NODE *next;
};

struct ordered_list
{
    ED base;
    NODE *start;
    DATATYPE sortingCriteria;
    int len;
};

static bool order_list_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    ORDERED_LIST *order_list = (ORDERED_LIST *)*self;
    bool ok = delete_ordered_list(&order_list);
    *self = NULL;

    return ok;
}

static bool ordered_list_insert_ed(ED *self, ITEM *item)
{
    return ordered_list_add_item((ORDERED_LIST *)self, item);
}

static void ordered_list_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const ORDERED_LIST *order_list = (const ORDERED_LIST *)self;

    for (const NODE *atual = order_list->start; atual != NULL; atual = atual->next)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE order_list_vtable =
    {
        order_list_destroy,
        ordered_list_insert_ed,
        ordered_list_for_each};

// funcoes

/**
 * @brief Cria ORDERED_LIST
 * @param[in] INT que indica a posicao do campo no vetor FIELDS do item usado para a ordenacao das lista
 * @return Ponteiro para a ORDERED_LIST ou NULL no caso de erros.
 */
extern ORDERED_LIST *create_ordered_list(void) // Colocar de volta o critério de Sorting
{
    int sortingCriteria = 0; // Apagar
    ORDERED_LIST *ordered_list = (ORDERED_LIST *)malloc(sizeof(ORDERED_LIST));

    if (ordered_list == NULL)
    {
        printf("Falha na alocacao da lista cruzada");
        return NULL;
    }

    ordered_list->len = 0;
    ordered_list->start = NULL;
    ordered_list->base.ed_vtable = &order_list_vtable;
    ordered_list->sortingCriteria = sortingCriteria;

    return ordered_list;
}

/**
 * @brief Deleta ORDERED_LIST
 * @param[out] Ponteiro duplo para ORDERED_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
bool delete_ordered_list(ORDERED_LIST **ordered_list)
{
    if (ordered_list == NULL || *ordered_list == NULL)
    {
        printf("ponteiro de lista ordenada invalido para delecao");
        return false;
    }

    NODE *current, *next;
    current = (*ordered_list)->start;
    next = current->next;

    for (int i = 0; i < (*ordered_list)->len; i++)
    {
        if (current == NULL) // redundancia caso o len tenha quebrado
        {
            printf("possivel erro na delecao da lista cruzada");
            break;
        }

        if (current->previous != NULL) // se tiver antecessor, tira os ponteiros nas duas direcoes
        {
            current->previous->next = NULL;
            current->previous = NULL;
        }
        if (current->next != NULL) // se tiver sucessor, tira os ponteiros nas duas direcoes
        {
            current->next->previous = NULL;
            current->next = NULL;
        }
        delete_item(&(current->item));
        current->item = NULL;
        free(current);

        current = next;

        if (current != NULL)
        {
            next = current->next;
        }
    }
    (*ordered_list)->start = NULL;
    free((*ordered_list)); // libera a estrutura principal e tira ponteiro pendente
    ordered_list = NULL;

    return true;
}

/**
 * @brief Adicona um ITEM na lista na posicao determinada pela ordenacao baseada no campo estabelecido na criacao da lista ordenada.
 * @param[out] Ponteiro  para a ORDERED_LIST.
 * @param[in] endereço do item a ser inserido na lista.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista, isso leva a um erro).
 */
bool ordered_list_add_item(ORDERED_LIST *ordered_list, ITEM *item)
{
    if (ordered_list == NULL || item == NULL)
    {
        printf("Item ou lista ordenada invalidas para insercao");
        return false;
    }

    NODE *no = (NODE *)malloc(sizeof(NODE));
    no->item = item;

    // caso de insercao caso nenhuma das duas listas tenha itens
    if (ordered_list->len == 0)
    {
        ordered_list->start = no;
        no->next = NULL;
        no->previous = NULL;
    }
    else // insercao padrao
    {
        NODE *aux; // ponteiro que vai ajudar a determinar em qual posicao inserir

        // insercao na lista-linha
        aux = ordered_list->start;

        for (int i = 0; i < ordered_list->len; i++) // cicles item per item, checking where the node should be placed
        {
            if (aux == NULL) // would indicate a possible error
            {
                break;
            }

            // pega qual tipo de comparacao usar
            if (item_get_type(item, ordered_list->sortingCriteria) == ITEM_STRING)
            { // comparing strings (strcmp)
                if (strcmp((char *)get_field_content(aux->item, ordered_list->sortingCriteria), (char *)get_field_content(item, ordered_list->sortingCriteria)) <= 0)
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    no->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        no->previous = NULL;
                        ordered_list->start = no;
                    }
                    else
                    {
                        no->previous = aux->previous;
                        aux->previous->next = no;
                    }
                    aux->previous = no;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (ordered_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = no;
                    no->previous = aux;
                    no->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if (item_get_type(item, ordered_list->sortingCriteria) == ITEM_BOOL)
            { // comparing boolean (true goes first, false goes later)
                if (*((bool *)get_field_content(aux->item, ordered_list->sortingCriteria)) == false &&
                    *((bool *)get_field_content(item, ordered_list->sortingCriteria)) == true) //
                {                                                                              // if the item of the position we are inspecting is false and the item is true, we position our new node behind it, in the border between the true and the false
                    no->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        no->previous = NULL;
                        ordered_list->start = no;
                    }
                    else
                    {
                        no->previous = aux->previous;
                        aux->previous->next = no;
                    }
                    aux->previous = no;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (ordered_list->len - 1)) // if the item is false or there are only true items in the list it is to be placed at the end of the list
                {
                    aux->next = no;
                    no->previous = aux;
                    no->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            // comparing numeric
            else if (item_get_type(item, ordered_list->sortingCriteria) == ITEM_INT)
            { // if its an integer
                if (*((int *)get_field_content(aux->item, ordered_list->sortingCriteria)) > *((int *)get_field_content(item, ordered_list->sortingCriteria)))
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    no->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        no->previous = NULL;
                        ordered_list->start = no;
                    }
                    else
                    {
                        no->previous = aux->previous;
                        aux->previous->next = no;
                    }
                    aux->previous = no;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (ordered_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = no;
                    no->previous = aux;
                    no->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if (item_get_type(item, ordered_list->sortingCriteria) == ITEM_DOUBLE)
            { // if its a double
                if (*((double *)get_field_content(aux->item, ordered_list->sortingCriteria)) > *((double *)get_field_content(item, ordered_list->sortingCriteria)))
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    no->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        no->previous = NULL;
                        ordered_list->start = no;
                    }
                    else
                    {
                        no->previous = aux->previous;
                        aux->previous->next = no;
                    }
                    aux->previous = no;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (ordered_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = no;
                    no->previous = aux;
                    no->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else
            {
                printf("erro na insercao na lista cruzada; tipo invalido");
                return false;
            }

            // if it reaches here, it checked the position related to the current node aux is pointing to and the place wasnt aproppriate, so we move onto the next node
            aux = aux->next;
        }
    }
    ordered_list->len += 1;

    return true;
}

/** As funcoes de remocao foram removidas por obsolecencia */

/**
 * @brief Busca por ITEM correspondente a KEY de ORDERED_LIST.
 * @param[in] Ponteiro para ORDERED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro para void pois o tipo do key e indeterminado).
 * @param[in] field - int que indica em qual campo o valor de key esta localizado.
 * @return Ponteiro para ITEM, NULL em caso de erros (lista/item inexistente ou nao encontrado, tipo de campo invalido, etc).
 */
ITEM *ordered_list_search(ORDERED_LIST *ordered_list, void *key, int field)
{
    NODE *aux = ordered_list->start;

    for (int i = 0; i < ordered_list->len; i++)
    {

        if (aux == NULL) // would indicate a possible error
        {
            break;
        }

        if (item_get_type(aux->item, field) == ITEM_STRING)
        { // looking for string (strcmp) - returns the first item found with the same string in the designated field
            if (strcmp((char *)get_field_content(aux->item, field), *((char **)key)) == 0)
            {
                return aux->item;
            }
        }
        else if (item_get_type(aux->item, field) == ITEM_BOOL)
        { // looking for boolean - returns the first item found with the same boolean value in the designated field
            if (*((bool *)get_field_content(aux->item, field)) == *((bool *)key))
            {
                return aux->item;
            }
        }
        // comparing numeric
        else if (item_get_type(aux->item, field) == ITEM_INT)
        { // looking for an integer  - returns the first item found with the equivalent int value in the designated field
            if (*((int *)get_field_content(aux->item, field)) == *((int *)key))
            {
                return aux->item;
            }
        }
        else if (item_get_type(aux->item, field) == ITEM_DOUBLE)
        { // looking for a double  - returns the first item found with the equivalent double value in the designated field
            if (*((double *)get_field_content(aux->item, field)) == *((double *)key))
            {
                return aux->item;
            }
        }
        else
        {
            printf("erro na busca na lista ordenada; tipo invalido");
            return false;
        }

        aux = aux->next;
    }

    printf("Item nao encontrado");
    return NULL;
}
