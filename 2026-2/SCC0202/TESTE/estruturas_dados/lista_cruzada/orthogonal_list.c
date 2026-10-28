/*
Breve descricao do entendimento da lista cruzada e da sua implementacao
(Esse topico foi discutido previamente com o Rudinei e tambem consultado brevemente por email com o Marcacini)

A representacao de lista cruzada que sera feita aqui pode ser descrita, na pratica, como duas listas encadeadas ordenadas sob criterios
diferentes de modo que elas podem formar uma representacao bidimensional que representa uma matriz esparsa em que a posicao de vertical
e horizontal de um item nessa matriz representa a posicao na ordenacao nas respectivas listas encadeadas ordenadas.

Devido a essa caracteristica, Algumas partes da lista cruzada implementada em aula seram retiradas, como por exemplo os ponteiros para
os itens acima (na mesma coluna) e a direita (na mesma linha), pois como a posicao do item na matriz representa sua posicao na ordenacao,
nao havera mais de um item por linha ou coluna.

Essa consequencia tambem permite que a lista cruzada cumpra o seu proposito de representar uma  matriz esparca o suficiente a ponto de ela
ser ineficiente de ser implementada como uma simples matriz sequencial, pois haveria muitos itens vazios. Considerando que a lista de
asteroides tem por volta de 89000 itens, uma matriz alocada sequencialmente ocuparia 89000*89000 espacos, com a maior parte desses ficando
vazios, enquanto essa matriz esparca ocuparia, efetivamente, 89000*2 para os nos das linhas e das colunas + 89000 para a alocacao dos itens,
que serao apontados pelas duas listas (totalizando 89000*3 itens alocados).

Uma observacao relevante e que, embora na pratica a lista cruzada sera composta por duas listas encadeadas separadas, como isso e uma forma
de implementacao de uma unica lista cruzada, os itens que sao adicionados e retiradas (caso fosse aplicavel) tem essas operacoes aplicadas nas duas listas simultaneamente
o que faz com que as listas ordenadas se comportem como uma unica lista. E, alem disso, ambas as listas apontam para os mesmos itens alocados
uma unica vez, com apenas os nos sendo alocados duas vezes (uma em cada lista) e apontando para esses itens de posicoes diferentes baseado nos
criterios de ordenacao.

Outra coisa importante sobre a ordenacao em si e que, como a listas listas da lista cruzada serao ordenadas com base nos criterios
providenciados no comando CROSS, como indicado no enunciado, as ordenacoes seram feitas via comparacoes com base no criterio providenciado
que e informado com base no enumerador datatype (dos arquivos item)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "orthogonal_list.h"
#include "../item.h"
#include "../estruturas_dados.h"

struct node
{
    ITEM *item;
    NODE *previous;
    NODE *next;
};

struct orthogonal_list
{
    ED base; // sempre o primeiro
    NODE *lineStart;
    DATATYPE lineSortingField;
    NODE *columnStart;
    DATATYPE columnSortingField;
    int len;
};

// prototipos
CROSS_LIST *create_cross_list(void); // Colocar de volta int lineCriteria, int columnCriteria
bool delete_cross_list(CROSS_LIST **cross_list);
bool add_item(CROSS_LIST *cross_list, ITEM *item);
ITEM *search(CROSS_LIST *linked_list, void *key, int field);

//===========================
static bool l_cruz_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    CROSS_LIST *l_cruz = (CROSS_LIST *)*self;
    bool ok = delete_cross_list(&l_cruz);
    *self = NULL;

    return ok;
}

static bool l_cruz_insert_ed(ED *self, ITEM *item)
{
    return add_item((CROSS_LIST *)self, item);
}

static void l_cruz_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const CROSS_LIST *query = (const CROSS_LIST *)self;

    for (const NODE *linha = query->lineStart; linha != NULL; linha = linha->next)
    {
        if (visit(linha->item, ctx) == false)
        {
            return;
        }
    }
}

static const ED_VTABLE l_cruz_vtable =
    {
        l_cruz_destroy,
        l_cruz_insert_ed,
        l_cruz_for_each};
//===========================

// funcoes

/**
 * @brief Cria CROSS_LIST
 * @param[in] INTs que indicam a posicao dos campos no vetor FIELDS do item usados para a ordenacao das linha e coluna respectivamente
 * @return Ponteiro para a CROSS_LIST ou NULL no caso de erros.
 */
// Colocar de volta int lineCriteria, int columnCriteria
CROSS_LIST *create_cross_list(void)
{
    CROSS_LIST *xList = (CROSS_LIST *)malloc(sizeof(CROSS_LIST));

    if (xList == NULL)
    {
        printf("Falha na alocacao da lista cruzada");
        return xList;
    }
    int lineCriteria = 0;   // APAGAR
    int columnCriteria = 0; // APAGAR
    xList->base.ed_vtable = &l_cruz_vtable;
    xList->len = 0;
    xList->lineStart = NULL;
    xList->lineSortingField = lineCriteria;
    xList->columnStart = NULL;
    xList->columnSortingField = columnCriteria;

    return xList;
}

/**
 * @brief Deleta CROSS_LIST
 * @param[out] Ponteiro duplo para CROSS_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
bool delete_cross_list(CROSS_LIST **cross_list)
{
    if (cross_list == NULL || *cross_list == NULL)
    {
        printf("ponteiro de lista cruzada invalido para delecao");
        return false;
    }

    NODE *current, *next;
    current = (*cross_list)->lineStart;
    next = current->next;

    for (int i = 0; i < (*cross_list)->len; i++) // passa pela lista-linha e vai liberando os nos e os itens
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
        free(current);

        current = next;

        if (current != NULL)
        {
            next = current->next;
        }
    }
    (*cross_list)->lineStart = NULL; // completa a liberacao da lista-linha

    current = (*cross_list)->columnStart;
    next = current->next;
    for (int i = 0; i < (*cross_list)->len; i++) // passa pela lista-coluna liberando seus nos e tirando ponteiros pendentes
    {
        if (current == NULL) // redundancia caso o len tenha quebrado
        {
            printf("possivel erro na delecao da lista cruzada");
            return false;
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
        current->item = NULL; // o item ja foi deletado no loop anterior, isso apenas tira o ponteiro pendente
        free(current);

        current = next;

        if (current != NULL)
        {
            next = current->next;
        }
    }
    (*cross_list)->columnStart = NULL; // completa a liberacao da lista-coluna

    free((*cross_list)); // libera a estrutura principal e tira ponteiro pendente
    cross_list = NULL;

    return true;
}

/**
 * @brief Adicona um ITEM nas lista-linha e lista-coluna na posicao determinada pela ordenacao da lista baseado nos campos estabelecidos na criacao da lista cruzada.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] endereço do item a ser inserido nas listas.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista-linha ou da lista-coluna, isso leva a um erro).
 */
bool add_item(CROSS_LIST *cross_list, ITEM *item)
{
    if (cross_list == NULL || item == NULL)
    {
        printf("Item ou lista cruzada invalidas para insercao");
        return false;
    }

    NODE *noLinha = (NODE *)malloc(sizeof(NODE));
    NODE *noColuna = (NODE *)malloc(sizeof(NODE));

    noLinha->item = item;
    noColuna->item = item;

    // caso de insercao caso nenhuma das duas listas tenha itens
    /*
    A principio, por agora, vai so funcionar com o asteroide, mas a framework para adaptar para o
    petroleo ja vai estar ai, teremos apenas que implementar um if ou algo do tipo que verifica
    uma variavel que fala qual banco de dados esta sendo usado
    */
    if (cross_list->len == 0)
    {
        cross_list->lineStart = noLinha;
        cross_list->columnStart = noColuna;

        noLinha->next = NULL;
        noLinha->previous = NULL;
        noColuna->next = NULL;
        noColuna->previous = NULL;
    }
    else // insercao padrao
    {
        /*A implementacao ordenada vai ser relativamente simples, quando for inserir um item
        o algoritmo vai so ir checando item a item se o campo especificado e o outro, se sim,
        passa para a proxima posicao
        como nao foi estabelecido como ordenar (crecente, decrescente, alfabetico, inverso, etc)
        eu vou so fazer ordenacao crescente pra numericos, alfabetico para strings
        como as strings do item sao padronizadas (letras maiusculass sempre aparecem na mesma
        pos, etc) um simples strcmp deve funcionar.
        */
        NODE *aux; // ponteiro que vai ajudar a determinar em qual posicao inserir

        // insercao na lista-linha
        aux = cross_list->lineStart;

        for (int i = 0; i < cross_list->len; i++) // cicles item per item, checking where the node should be placed
        {
            if (aux == NULL) // would indicate a possible error
            {
                break;
            }

            // check field, then determine where to put it, then adjust the connecting pointers

            // pega qual tipo de comparacao usar
            if (item_get_type(item, cross_list->lineSortingField) == ITEM_STRING)
            { // comparing strings (strcmp)
                if (strcmp((char *)get_field_content(aux->item, cross_list->lineSortingField), (char *)get_field_content(item, cross_list->lineSortingField)) <= 0)
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noLinha->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noLinha->previous = NULL;
                        cross_list->lineStart = noLinha;
                    }
                    else
                    {
                        noLinha->previous = aux->previous;
                        aux->previous->next = noLinha;
                    }
                    aux->previous = noLinha;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if (item_get_type(item, cross_list->lineSortingField) == ITEM_BOOL)
            { // comparing boolean (true goes first, false goes later)
                if (*((bool *)get_field_content(aux->item, cross_list->lineSortingField)) == false &&
                    *((bool *)get_field_content(item, cross_list->lineSortingField)) == true) //
                {                                                                             // if the item of the position we are inspecting is false and the item is true, we position our new node behind it, in the border between the true and the false
                    noLinha->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noLinha->previous = NULL;
                        cross_list->lineStart = noLinha;
                    }
                    else
                    {
                        noLinha->previous = aux->previous;
                        aux->previous->next = noLinha;
                    }
                    aux->previous = noLinha;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is false or there are only true items in the list it is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            // comparing numeric
            else if (item_get_type(item, cross_list->lineSortingField) == ITEM_INT)
            { // if its an integer
                if (*((int *)get_field_content(aux->item, cross_list->lineSortingField)) > *((int *)get_field_content(item, cross_list->lineSortingField)))
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noLinha->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noLinha->previous = NULL;
                        cross_list->lineStart = noLinha;
                    }
                    else
                    {
                        noLinha->previous = aux->previous;
                        aux->previous->next = noLinha;
                    }
                    aux->previous = noLinha;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if (item_get_type(item, cross_list->lineSortingField) == ITEM_DOUBLE)
            { // if its a double
                if (*((double *)get_field_content(aux->item, cross_list->lineSortingField)) > *((double *)get_field_content(item, cross_list->lineSortingField)))
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noLinha->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noLinha->previous = NULL;
                        cross_list->lineStart = noLinha;
                    }
                    else
                    {
                        noLinha->previous = aux->previous;
                        aux->previous->next = noLinha;
                    }
                    aux->previous = noLinha;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

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

        // insercao na lista-coluna
        aux = cross_list->columnStart;

        for (int i = 0; i < cross_list->len; i++) // cicles item per item, checking where the node should be placed
        {
            if (aux == NULL) // would indicate a possible error
            {
                break;
            }

            // check field, then determine where to put it, then adjust the connecting pointers

            // pega qual tipo de comparacao usar
            if (item_get_type(item, cross_list->columnSortingField) == ITEM_STRING)
            { // comparing strings (strcmp)
                if (strcmp((char *)get_field_content(aux->item, cross_list->columnSortingField), (char *)get_field_content(item, cross_list->columnSortingField)) <= 0)
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noColuna->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noColuna->previous = NULL;
                        cross_list->columnStart = noColuna;
                    }
                    else
                    {
                        noColuna->previous = aux->previous;
                        aux->previous->next = noColuna;
                    }
                    aux->previous = noColuna;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if (item_get_type(item, cross_list->columnSortingField) == ITEM_BOOL)
            { // comparing boolean (true goes first, false goes later)
                if (*((bool *)get_field_content(aux->item, cross_list->columnSortingField)) == false &&
                    *((bool *)get_field_content(item, cross_list->columnSortingField)) == true) //
                {                                                                               // if the item of the position we are inspecting is false and the item is true, we position our new node behind it, in the border between the true and the false
                    noColuna->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noColuna->previous = NULL;
                        cross_list->columnStart = noColuna;
                    }
                    else
                    {
                        noColuna->previous = aux->previous;
                        aux->previous->next = noColuna;
                    }
                    aux->previous = noColuna;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is false or there are only true items in the list it is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            // comparing numeric
            else if (item_get_type(item, cross_list->columnSortingField) == ITEM_INT)
            { // if its an integer
                if (*((int *)get_field_content(aux->item, cross_list->columnSortingField)) > *((int *)get_field_content(item, cross_list->columnSortingField)))
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noColuna->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noColuna->previous = NULL;
                        cross_list->columnStart = noColuna;
                    }
                    else
                    {
                        noColuna->previous = aux->previous;
                        aux->previous->next = noColuna;
                    }
                    aux->previous = noColuna;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if (item_get_type(item, cross_list->columnSortingField) == ITEM_DOUBLE)
            { // if its a double
                if (*((double *)get_field_content(aux->item, cross_list->columnSortingField)) > *((double *)get_field_content(item, cross_list->columnSortingField)))
                { // if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noColuna->next = aux;
                    if (i == 0) // if the item is to be placed behind the current first node;
                    {
                        noColuna->previous = NULL;
                        cross_list->columnStart = noColuna;
                    }
                    else
                    {
                        noColuna->previous = aux->previous;
                        aux->previous->next = noColuna;
                    }
                    aux->previous = noColuna;

                    break; // if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }

                if (i == (cross_list->len - 1)) // if the item is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

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

    cross_list->len += 1;

    return true;
}

/** As funcoes de remocao foram removidas por obsolecencia*/
ITEM *search(CROSS_LIST *cross_list, void *key, int field)
{
    // determines if the search should happen on the line-list or column-list
    NODE *aux = cross_list->lineStart; // will look in the line-list by default, unless the field is the field used in the sorting of the column-line
    if (field == cross_list->columnSortingField)
    { // key being looked for is the same field as the column sorting field (so it should be near the top)
        aux = cross_list->columnStart;
    }

    for (int i = 0; i < cross_list->len; i++)
    {

        if (aux == NULL) // would indicate a possible error
        {
            break;
        }

        // valid for asteroid only
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
            printf("erro na busca na lista cruzada; tipo invalido");
            return false;
        }

        aux = aux->next;
    }

    printf("Item nao encontrado");
    return NULL;
}

/**
 aonde eu parei
 talvez vale a pena de separar a comparacao entre os dois campos em uma funcao separada, talvez, nao tenho certeza.

 eu vou ter que adaptar o codigo depois para usar os item_get, ao inves de acessar os campos diretamente, quando ficar pronto, pro enquanto deixar assim mesmo
 */
