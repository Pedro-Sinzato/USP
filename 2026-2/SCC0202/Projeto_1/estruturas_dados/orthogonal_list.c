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
    NODE* startLine;
    DATATYPE lineSortingType;
    NODE* startColumn;
    DATATYPE columnSortingType;
    int len; 
};

//prototipos

CROSS_LIST *create_cross_list(DATATYPE lineCriteria, DATATYPE columnCriteria);
bool delete_cross_list(CROSS_LIST **cross_list);
bool add_item(CROSS_LIST *cross_list, ITEM* item);
bool remove_start(CROSS_LIST *cross_list, int listIndicator);
bool remove_search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);
bool remove_end(CROSS_LIST *cross_list, int listIndicator);
ITEM *search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);


//funcoes

/**
 * @brief Cria CROSS_LIST
 * @param[in] tipos de dado usados para a ordenacao das linha e coluna respectivamente
 * @return Ponteiro para a CROSS_LIST ou NULL no caso de erros.
 */
CROSS_LIST *create_cross_list(DATATYPE lineCriteria, DATATYPE columnCriteria);

/**
 * @brief Deleta CROSS_LIST
 * @param[out] Ponteiro duplo para CROSS_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
bool delete_cross_list(CROSS_LIST **cross_list);

/**
 * @brief Adicona um ITEM nas lista-linha e lista-coluna na posicao determinada pela ordenacao da lista baseado nos lineCriteria e columnCriteria.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] endereço do item a ser inserido nas listas.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista-linha ou da lista-coluna, isso leva a um erro).
 */
bool add_item(CROSS_LIST *cross_list, ITEM* item);

/**
 * @brief Remove o ITEM na primeira posicao da lista indicada (lista-linha ou lista-coluna).
 * @param[out] Ponteiro  para a CROSS_LIST.
 * @param[in] numero que indica se o item a ser removido e o primeiro da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 */
bool remove_start(CROSS_LIST *cross_list, int listIndicator);
/**
 * @brief Remove ITEM correspondente a KEY de CROSS_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser removido deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 */
bool remove_search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);
/**
 * @brief Remove ITEM no fim de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] numero que indica se o item a ser removido e o ultimo da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 */
bool remove_end(CROSS_LIST *cross_list, int listIndicator);

/**
 * @brief Busca por ITEM correspondente a KEY de LINKED_LIST.
 * @param[in] Cópia de LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser retornado deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return Ponteiro para ITEM, NULL em caso de erros (lista/item inexistente ou nao encontrado, listIndicator invalido, etc).
 */
ITEM *search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);