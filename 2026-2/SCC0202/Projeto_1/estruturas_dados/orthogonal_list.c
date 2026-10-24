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
que serao apontados pelas duas listas.

Uma observacao relevante e que, embora na pratica a lista cruzada sera composta por duas listas encadeadas separadas, como isso e uma forma 
de implementacao de uma unica lista cruzada, os itens que sao adicionados e retiradas tem essas operacoes aplicadas nas duas simultaneamente
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
#include "../camposNasa.h"
#include "item.c"

struct node
{
    ITEM* item;
    NODE* previous;
    NODE* next;
};

typedef struct node NODE;

struct orthogonal_list
{
    NODE* lineStart;
    DATATYPE lineSortingField;
    NODE* columnStart;
    DATATYPE columnSortingField;
    int len; 
};


//prototipos
CROSS_LIST *create_cross_list(int lineCriteria, int columnCriteria);
bool delete_cross_list(CROSS_LIST **cross_list);
bool add_item(CROSS_LIST *cross_list, ITEM* item);
bool remove_start(CROSS_LIST *cross_list, int listIndicator);
bool remove_search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);
bool remove_end(CROSS_LIST *cross_list, int listIndicator);
ITEM *search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);


//funcoes

/**
 * @brief Cria CROSS_LIST
 * @param[in] INTs que indicam a posicao dos campos no vetor FIELDS do item usados para a ordenacao das linha e coluna respectivamente
 * @return Ponteiro para a CROSS_LIST ou NULL no caso de erros.
 */
CROSS_LIST *create_cross_list(int lineCriteria, int columnCriteria)
{
    CROSS_LIST* xList = (CROSS_LIST*)malloc(sizeof(CROSS_LIST));

    if(xList == NULL)
    {
        printf("Falha na alocacao da lista cruzada");
        return xList;
    }

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
    if(cross_list == NULL || *cross_list == NULL)
    {
        printf("ponteiro de lista cruzada invalido para delecao");
        return false;
    }

   NODE *current, *next;
   current = (*cross_list)->lineStart;
   next = current->next;

    for (int i = 0; i < (*cross_list)->len; i++) //passa pela lista-linha e vai liberando os nos e os itens
    {
        if(current == NULL) //redundancia caso o len tenha quebrado
        {
            printf("possivel erro na delecao da lista cruzada");
            break;
        }

        if(current->previous != NULL) //se tiver antecessor, tira os ponteiros nas duas direcoes
        {
            current->previous->next = NULL;
            current->previous = NULL;
        }
        if(current->next != NULL) //se tiver sucessor, tira os ponteiros nas duas direcoes
        {
            current->next->previous = NULL;
            current->next = NULL;
        }
        delete_item(&(current->item));
        free(current);

        current = next;
        next = current->next;
        
        if(next == NULL) //se encontrar NULL no proximo tem uma chance da variavel len ter quebrado, isso e apeans uma redundancia
        {
            printf("possivel erro na delecao da lista cruzada");
            break;
        }
    }
    (*cross_list)->lineStart = NULL; //completa a liberacao da lista-linha

    for(int i = 0; i < (*cross_list)->len; i++) //passa pela lista-coluna liberando seus nos e tirando ponteiros pendentes
    {
        if(current == NULL) //redundancia caso o len tenha quebrado
        {
            printf("possivel erro na delecao da lista cruzada");
            return false;
        }

        if(current->previous != NULL) //se tiver antecessor, tira os ponteiros nas duas direcoes
        {
            current->previous->next = NULL;
            current->previous = NULL;
        }
        if(current->next != NULL) //se tiver sucessor, tira os ponteiros nas duas direcoes
        {
            current->next->previous = NULL;
            current->next = NULL;
        }
        current->item = NULL; //o item ja foi deletado no loop anterior, isso apenas tira o ponteiro pendente
        free(current);

        current = next;
        next = current->next;
        
        if(next == NULL) //se encontrar NULL no proximo tem uma chance da variavel len ter quebrado, isso e apeans uma redundancia
        {
            printf("possivel erro na delecao da lista cruzada"); //isso pode indicar um erro na variavel len ou em algum ponteiro ao longo da lista
            return false;
        }
    }
    (*cross_list)->columnStart = NULL; //completa a liberacao da lista-coluna

    free((*cross_list)); //libera a estrutura principal e tira ponteiro pendente
    cross_list = NULL;

    return true;
}

/**
 * @brief Adicona um ITEM nas lista-linha e lista-coluna na posicao determinada pela ordenacao da lista baseado nos campos estabelecidos na criacao da lista cruzada.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] endereço do item a ser inserido nas listas.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista-linha ou da lista-coluna, isso leva a um erro).
 */
bool add_item(CROSS_LIST *cross_list, ITEM* item)
{
    if(cross_list == NULL || item == NULL)
    {
        printf("Item ou lista cruzada invalidas para insercao");
        return false;
    }

    NODE* noLinha = (NODE*)malloc(sizeof(NODE));
    NODE* noColuna = (NODE*)malloc(sizeof(NODE));

    noLinha->item = item;
    noColuna->item = item;

    //caso de insercao caso nenhuma das duas listas tenha itens
    /*
    A principio, por agora, vai so funcionar com o asteroide, mas a framework para adaptar para o
    petroleo ja vai estar ai, teremos apenas que implementar um if ou algo do tipo que verifica
    uma variavel que fala qual banco de dados esta sendo usado
    */
    if(cross_list->len == 0)
    {
        cross_list->lineStart = noLinha;
        cross_list->columnStart = noColuna;

        noLinha->next = NULL;
        noLinha->previous = NULL;
        noColuna->next = NULL;
        noColuna->previous = NULL;  
    }
    else //insercao padrao
    {
        /*A implementacao ordenada vai ser relativamente simples, quando for inserir um item
        o algoritmo vai so ir checando item a item se o campo especificado e o outro, se sim, 
        passa para a proxima posicao
        como nao foi estabelecido como ordenar (crecente, decrescente, alfabetico, inverso, etc)
        eu vou so fazer ordenacao crescente pra numericos, alfabetico para strings
        como as strings do item sao padronizadas (letras maiusculass sempre aparecem na mesma
        pos, etc) um simples strcmp deve funcionar.
        */
        NODE* aux; //ponteiro que vai ajudar a determinar em qual posicao inserir

        //insercao na lista-linha
        aux = cross_list->lineStart;

        for(int i = 0; i < cross_list->len; i++)//cicles item per item, checking where the node should be placed
        {
            if(aux == NULL && i != (cross_list->len - 1)) //would indicate a possible error
            {
                break;
            }

            //check field, then determine where to put it, then adjust the connecting pointers
            
            //pega qual tipo de comparacao usar
            if((int)(cross_list->lineSortingField) >= 0 && (int)(cross_list->lineSortingField) <= 4)
            { //comparing strings (strcmp)
                if(strcmp(aux->item->fields[cross_list->lineSortingField].v.s, item->fields[cross_list->lineSortingField].v.s) <= 0)
                {// if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noLinha->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(i ==(cross_list->len - 1)) //if the item is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if ((int)(cross_list->lineSortingField) >= 5 && (int)(cross_list->lineSortingField) <= 7)
            {//comparing boolean (true goes first, false goes later)
                if(aux->item->fields[cross_list->lineSortingField].v.b == false &&
                     item->fields[cross_list->lineSortingField].v.b == true) //
                {// if the item of the position we are inspecting is false and the item is true, we position our new node behind it, in the border between the true and the false
                    noLinha->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(item->fields[cross_list->lineSortingField].v.b == false) //if the item is false it is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }   
            }
            //comparing numeric
            else if((int)cross_list->lineSortingField >= 8 && (int)cross_list->lineSortingField <= 11)
            {//if its an integer
                if(aux->item->fields[cross_list->lineSortingField].v.i > item->fields[cross_list->lineSortingField].v.i)
                {// if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noLinha->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(i ==(cross_list->len - 1)) //if the item is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if ((int)cross_list->lineSortingField >= 12 && (int)cross_list->lineSortingField <= 19)
            {//if its a double
                if(aux->item->fields[cross_list->lineSortingField].v.d > item->fields[cross_list->lineSortingField].v.d)
                {// if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noLinha->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(i ==(cross_list->len - 1)) //if the item is to be placed at the end of the list
                {
                    aux->next = noLinha;
                    noLinha->previous = aux;
                    noLinha->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else
            {
                printf("erro na insercao na lista cruzada; tipo invalido");
                return false;
            }

            //if it reaches here, it checked the position related to the current node aux is pointing to and the place wasnt aproppriate, so we move onto the next node
            aux = aux->next; 
        }


        //insercao na lista-coluna
        aux = cross_list->columnStart;

        for(int i = 0; i < cross_list->len; i++)//cicles item per item, checking where the node should be placed
        {
            if(aux == NULL && i != (cross_list->len - 1)) //would indicate a possible error
            {
                break;
            }

            //check field, then determine where to put it, then adjust the connecting pointers
            
            //pega qual tipo de comparacao usar
            if((int)(cross_list->columnSortingField) >= 0 && (int)(cross_list->columnSortingField) <= 4)
            { //comparing strings (strcmp)
                if(strcmp(aux->item->fields[cross_list->columnSortingField].v.s, item->fields[cross_list->columnSortingField].v.s) <= 0)
                {// if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noColuna->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(i ==(cross_list->len - 1)) //if the item is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if ((int)(cross_list->columnSortingField) >= 5 && (int)(cross_list->columnSortingField) <= 7)
            {//comparing boolean (true goes first, false goes later)
                if(aux->item->fields[cross_list->columnSortingField].v.b == false &&
                     item->fields[cross_list->columnSortingField].v.b == true) //
                {// if the item of the position we are inspecting is false and the item is true, we position our new node behind it, in the border between the true and the false
                    noColuna->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(item->fields[cross_list->columnSortingField].v.b == false) //if the item is false it is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }   
            }
            //comparing numeric
            else if((int)cross_list->columnSortingField >= 8 && (int)cross_list->columnSortingField <= 11)
            {//if its an integer
                if(aux->item->fields[cross_list->columnSortingField].v.i > item->fields[cross_list->columnSortingField].v.i)
                {// if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noColuna->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(i ==(cross_list->len - 1)) //if the item is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else if ((int)cross_list->columnSortingField >= 12 && (int)cross_list->columnSortingField <= 19)
            {//if its a double
                if(aux->item->fields[cross_list->columnSortingField].v.d > item->fields[cross_list->columnSortingField].v.d)
                {// if the item of the position we are inspecting is bigger than the item to be inserted, we position our new node behind it
                    noColuna->next = aux;
                    if(i == 0) //if the item is to be placed behind the current first node;
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
                    
                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
                
                if(i ==(cross_list->len - 1)) //if the item is to be placed at the end of the list
                {
                    aux->next = noColuna;
                    noColuna->previous = aux;
                    noColuna->next = NULL;

                    break; //if the program reaches here, it has placed the item, so the loop isnt necessary anymore
                }
            }
            else
            {
                printf("erro na insercao na lista cruzada; tipo invalido");
                return false;
            }

            //if it reaches here, it checked the position related to the current node aux is pointing to and the place wasnt aproppriate, so we move onto the next node
            aux = aux->next; 
        }
    }

    cross_list->len += 1;

    return true;
}

/** As funcoes de remocao foram removidas por obsolecencia
 * @brief Remove o ITEM na primeira posicao da lista indicada (lista-linha ou lista-coluna).
 * @param[out] Ponteiro  para a CROSS_LIST.
 * @param[in] numero que indica se o item a ser removido e o primeiro da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.

bool remove_start(CROSS_LIST *cross_list, int listIndicator);
/**
 * @brief Remove ITEM correspondente a KEY de CROSS_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser removido deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.

bool remove_search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);
/**
 * @brief Remove ITEM no fim de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] numero que indica se o item a ser removido e o ultimo da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 bool remove_end(CROSS_LIST *cross_list, int listIndicator);
 */

/**
 * @brief Busca por ITEM correspondente a KEY de LINKED_LIST.
 * @param[in] Cópia de LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser retornado deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return Ponteiro para ITEM, NULL em caso de erros (lista/item inexistente ou nao encontrado, listIndicator invalido, etc).
 */
ITEM *search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);


/**
 aonde eu parei
 eu tenho que implementar a funcao de busca, e talvez vale a pena de separar a comparacao entre os dois campos em
 uma funcao separada, talvez, nao tenho certeza.
 */
