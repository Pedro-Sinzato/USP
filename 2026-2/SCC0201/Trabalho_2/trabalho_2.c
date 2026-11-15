/**
 * @file main.c
 * @brief Projeto prático 2: dicionário de palavras em tabela HASH estática
 * fechada, com SONDAGEM QUADRÁTICA.
 * @author Pedro Sinzato
 *
 * Arquivo organizado nessa ordem:
 *   1. HASH_ITEM  (declarações)
 *   2. HASH_TABLE (declarações)
 *   3. main e funções auxiliares da leitura
 *   4. HASH_ITEM  (implementação)
 *   5. HASH_TABLE (implementação)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ======================================================================
 * 1. HASH_ITEM (declarações)
 * ====================================================================== */

/**
 * @def MAX_WORD_SIZE
 * Tamanho máximo de uma palavra, 30 + \0
 */
#define MAX_WORD_SIZE 31

/**
 * @struct HASH_ITEM
 * @brief Guarda WORD e parâmetros de HASH ITEM.
 * @var HASH_ITEM::n_aparicoes
 * Quantas vezes essa palavra apareceu
 * @var HASH_ITEM::is_empty
 * Se aquele espaço em HASH_TABLE nunca foi usado
 * @var HASH_ITEM::removido
 * Se aquele espaço continha um ITEM que foi removido
 * @var HASH_ITEM::word
 * Palavra guardada por HASH_ITEM
 */
typedef struct hash_item
{
    int n_aparicoes;

    bool is_empty;
    bool removido;
    char word[MAX_WORD_SIZE]; // 30 + \0
}HASH_ITEM;

/**
 * @brief Preenche um espaço de HASH_TABLE com a palavra (n_aparicoes = 1).
 * @param[out] Ponteiro para o HASH_ITEM
 * @param[in] Ponteiro para WORD
 * @return True se conseguir e False se não.
 */
extern bool create_hash_item(HASH_ITEM *hash_item, const char *word);
/**
 * @brief Marca o HASH_ITEM como removido (is_empty = false, removido = true).
 * @param[in,out] Ponteiro para HASH_ITEM
 * @return True se conseguir e False se não.
 */
extern bool delete_hash_item(HASH_ITEM *hash_item);

/**
 * @brief Soma 1 em n_aparicoes (palavra repetida).
 * @param[in,out] hash_item Ponteiro para HASH_ITEM
 * @return True se conseguir e False se não.
 */
extern bool increment_aparicoes(HASH_ITEM *hash_item);

/**
 * @brief Funções de Get para os campos de HASH_ITEM.
 * @param[in] hash_item Ponteiro para HASH_ITEM
 * @return Valor guardado em HASH_ITEM.
 */
extern bool is_empty(const HASH_ITEM *hash_item);
extern bool is_removed(const HASH_ITEM *hash_item);
extern int get_aparicoes(const HASH_ITEM *hash_item);
extern const char *get_word(const HASH_ITEM *hash_item);

/* ======================================================================
 * 2. HASH_TABLE (declarações)
 * ====================================================================== */

/**
 * @struct HASH_TABLE
 * @brief Guarda um array de HASH_ITEM e parâmetros de HASH_TABLE
 * @var HASH_TABLE::hash
 * Array de HASH_ITEM
 * @var HASH_TABLE::most_frequent
 * Ponteiro para o HASH_ITEM com maior aparições
 * @var HASH_TABLE::max_hash_size
 * Tamanho máximo do array de HASH_ITEM
 * @var HASH_TABLE::diff_words
 * Quantidade de palavras diferentes no array de HASH_ITEM
 * @var HASH_TABLE::const_um
 * Constante um da SONDAGEM QUADRÁTICA
 * @var HASH_TABLE::const_dois
 * Constante dois da SONDAGEM QUADRÁTICA
 */
typedef struct hash_table HASH_TABLE;

/**
 * @brief Cria HASH_TABLE.
 * @param[in] hash_size Tamanho máximo da HASH_TABLE
 * @param[in] Constante um da SONDAGEM QUADRÁTICA
 * @param[in] Constante dois da SONDAGEM QUADRÁTICA
 * @return NULL se não conseguir.
 */
extern HASH_TABLE *create_hash_table(int hash_size, int const_um, int const_dois);
/**
 * @brief Deleta HASH_TABLE.
 * @param[out] Ponteiro duplo para HASH_TABLE
 * @return True se conseguir e False se não.
 */
extern bool delete_hash_table(HASH_TABLE **hash_table);

/**
 * @brief Insere a palavra, soma uma aparição se já existir.
 * @param[in,out] hash_table Ponteiro para HASH_TABLE
 * @param[in] word Palavra (até 30 caracteres)
 * @return True se conseguir e False se não.
 */
extern bool insert_hash_table(HASH_TABLE *hash_table, const char *word);
/**
 * @brief Remove a palavra da HASH_TABLE (todas as aparições).
 * @param[in,out] Ponteiro para HASH_TABLE
 * @param[in] Palavra que deve ser removida
 * @return True se removeu e False se não.
 */
extern bool remove_hash_table(HASH_TABLE *hash_table, const char *word);

/**
 * @brief Busca a palavra na HASH_TABLE.
 * @param[in] Ponteiro para HASH_TABLE
 * @param[in] Palavra procurada
 * @return Ponteiro para o HASH_ITEM ou NULL se não existir.
 */
extern HASH_ITEM *search_hash_table(HASH_TABLE *hash_table, const char *word);

/**
 * @brief Get número de palavras diferentes no array de HASH_ITEM.
 * @param[in] Ponteiro para HASH_TABLE
 * @return -1 se o ponteiro for inválido.
 */
extern int get_diff_words(HASH_TABLE *hash_table);

/**
 * @brief Get a palavra mais frequente. Em empate, a primeira em ordem alfabética.
 * @param[in] hash_table Ponteiro para HASH_TABLE
 * @return Ponteiro para o HASH_ITEM dentro da tabela, ou NULL se a tabela
 * estiver vazia ou o ponteiro for inválido.
 */
extern HASH_ITEM *get_most_frequent(HASH_TABLE *hash_table);

/**
 * @brief Imprime cada palavra e a sua posição na tabela, na ordem do array.
 * Formato de cada linha: "palavra posição".
 * @param[in] hash_table Ponteiro para HASH_TABLE
 */
extern void print_hash_table(HASH_TABLE *hash_table);

/* ======================================================================
 * 3. main e funções auxiliares da leitura
 * ====================================================================== */

#define MAX_LINHA 120002 //120000 + \n + \0

static bool is_separator(char c);
static bool insert_line(HASH_TABLE *hash_table, const char *line);
static void discard_rest_of_line(void);

int main(void)
{
    int size_hash = 0;
    int parameter_one = 0;
    int parameter_two = 0;

    if(scanf("%d %d %d", &size_hash, &parameter_one, &parameter_two) != 3)
    {
        return EXIT_FAILURE;
    }

    HASH_TABLE *hash_table = create_hash_table(size_hash, parameter_one, parameter_two);
    if(hash_table == NULL)
    {
        return EXIT_FAILURE;
    }

    static char linha[MAX_LINHA];

    int t_integer;
    while(scanf("%d", &t_integer) == 1 && t_integer != 0)
    {
        if(t_integer == 1)
        {
            // Lê UMA linha de texto e insere cada palavra no dicionário
            discard_rest_of_line(); // Da linha anterior '\n'
            if(fgets(linha, sizeof(linha), stdin) != NULL && !insert_line(hash_table, linha))
            {
                fprintf(stderr, "Alguma palavra da linha nao foi inserida.\n");
            }
        }
        else if(t_integer == 2)
        {
            HASH_ITEM *mais_frequente = get_most_frequent(hash_table);
            if(mais_frequente == NULL)
            {
                printf("nenhuma palavra encontrada\n");
            }
            else
            {
                printf("foram encontradas %d palavras diferentes\n", get_diff_words(hash_table));
                printf("palavra mais frequente = %s, encontrada %d vezes\n",
                       get_word(mais_frequente), get_aparicoes(mais_frequente));
            }
        }
        else if(t_integer == 3)
        {
            int n_palavras = 0;
            char palavra[MAX_WORD_SIZE];

            scanf("%d", &n_palavras);
            for(int i = 0; i < n_palavras; i++)
            {
                scanf("%30s", palavra); // 30 = MAX_WORD_SIZE - 1
                HASH_ITEM *hash_item = search_hash_table(hash_table, palavra);
                if(hash_item == NULL)
                {
                    printf("%s nao encontrada\n", palavra);
                }
                else
                {
                    printf("%s encontrada %d\n", palavra, get_aparicoes(hash_item));
                }
            }
        }
        else if(t_integer == 4)
        {
            int n_palavras = 0;
            char palavra[MAX_WORD_SIZE];

            scanf("%d", &n_palavras);
            for(int i = 0; i < n_palavras; i++)
            {
                scanf("%30s", palavra);
                if(remove_hash_table(hash_table, palavra))
                {
                    printf("%s removida\n", palavra);
                }
                else
                {
                    printf("%s nao encontrada\n", palavra);
                }
            }
        }
        else if(t_integer == 5)
        {
            printf("imprimindo tabela hash\n");
            print_hash_table(hash_table);
            printf("fim da tabela hash\n");
        }
    }

    if(!delete_hash_table(&hash_table))
    {
        fprintf(stderr, "Falha ao deletar a tabela hash;\n");
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Remove o \n da linha anterior.
 */
static void discard_rest_of_line(void)
{
    int c;
    do
    {
        c = getchar();
    } while(c != '\n' && c != EOF);
}

/**
 * @brief Quebra a linha em palavras e insere cada uma em HASH_TABLE.
 * @param[in,out] Ponteiro para HASH_TABLE
 * @param[in] Linha terminada em '\0'
 * @return True se conseguir, False se não.
 */
static bool insert_line(HASH_TABLE *hash_table, const char *line)
{
    if(hash_table == NULL || line == NULL)
    {
        return false;
    }

    bool ok = true;
    char word[MAX_WORD_SIZE];
    const char *p = line;

    while(*p != '\0')
    {
        while(is_separator(*p))
        {
            p++;
        }
        if(*p == '\0')
        {
            break;
        }

        const char *inicio = p;
        while(*p != '\0' && !is_separator(*p))
        {
            p++;
        }

        size_t tamanho = (size_t)(p - inicio);
        if(tamanho >= MAX_WORD_SIZE)
        {
            ok = false; // não cabe em HASH_ITEM::word
            continue;
        }

        memcpy(word, inicio, tamanho);
        word[tamanho] = '\0';

        if(!insert_hash_table(hash_table, word))
        {
            ok = false;
        }
    }

    return ok;
}

static bool is_separator(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* ======================================================================
 * 4. HASH_ITEM (implementação)
 * ====================================================================== */

extern bool create_hash_item(HASH_ITEM *hash_item, const char *word)
{
    if(hash_item == NULL || word == NULL)
    {
        return false;
    }

    hash_item->n_aparicoes = 1;
    hash_item->is_empty = false;
    hash_item->removido = false;
    strncpy(hash_item->word, word, MAX_WORD_SIZE - 1);
    hash_item->word[MAX_WORD_SIZE - 1] = '\0';

    return true;
}
extern bool delete_hash_item(HASH_ITEM *hash_item)
{
    if(hash_item == NULL)
    {
        return false;
    }

    hash_item->n_aparicoes = 0;
    hash_item->is_empty = false;
    hash_item->removido = true;

    return true;
}

extern bool increment_aparicoes(HASH_ITEM *hash_item)
{
    if(hash_item == NULL)
    {
        return false;
    }

    hash_item->n_aparicoes++;

    return true;
}

extern bool is_empty(const HASH_ITEM *hash_item)
{
    return hash_item == NULL ? true : hash_item->is_empty;
}
extern bool is_removed(const HASH_ITEM *hash_item)
{
    return hash_item == NULL ? false : hash_item->removido;
}
extern int get_aparicoes(const HASH_ITEM *hash_item)
{
    return hash_item == NULL ? 0 : hash_item->n_aparicoes;
}
extern const char *get_word(const HASH_ITEM *hash_item)
{
    return hash_item == NULL ? NULL : hash_item->word;
}

/* ======================================================================
 * 5. HASH_TABLE (implementação)
 * ====================================================================== */

struct hash_table
{
    HASH_ITEM *hash;
    HASH_ITEM *most_frequent;
    int max_hash_size;
    int diff_words;

    int const_um;
    int const_dois;
};

static int hash_code(const char *word, int hash_size);
static int sondagem_quadratica(const HASH_TABLE *hash_table, const char *word, int hash_inicial, int *posicao_livre);
static void recalcular_most_frequent(HASH_TABLE *hash_table);
static bool is_more_frequent(const HASH_ITEM *item, const HASH_ITEM *outro);

extern HASH_TABLE *create_hash_table(int max_hash_size, int const_um, int const_dois)
{
    if(max_hash_size <= 0)
    {
        return NULL;
    }

    // calloc zera tudo
    HASH_TABLE *hash_table = calloc(1, sizeof(HASH_TABLE));
    if(hash_table == NULL)
    {
        return NULL;
    }

    hash_table->hash = calloc(max_hash_size, sizeof(HASH_ITEM));
    if(hash_table->hash == NULL)
    {
        free(hash_table);
        return NULL;
    }

    for(int i = 0; i < max_hash_size; i++)
    {
        hash_table->hash[i].is_empty = true;
    }

    hash_table->const_um = const_um;
    hash_table->const_dois = const_dois;
    hash_table->max_hash_size = max_hash_size;

    return hash_table;
}

extern bool delete_hash_table(HASH_TABLE **hash_table)
{
    if(hash_table == NULL || *hash_table == NULL)
    {
        return false;
    }

    free((*hash_table)->hash);
    free(*hash_table);
    *hash_table = NULL;

    return true;
}

extern bool insert_hash_table(HASH_TABLE *hash_table, const char *word)
{
    if(hash_table == NULL || word == NULL)
    {
        return false;
    }

    // palavra maior que 30 caracteres não cabe em HASH_ITEM::word
    if(strlen(word) >= MAX_WORD_SIZE)
    {
        return false;
    }

    int hash_inicial = hash_code(word, hash_table->max_hash_size);
    int posicao_livre;
    int posicao = sondagem_quadratica(hash_table, word, hash_inicial, &posicao_livre);

    HASH_ITEM *item;
    if(posicao != -1)
    {
        // palavra repetida
        item = &hash_table->hash[posicao];
        increment_aparicoes(item);
    }
    else
    {
        // palavra nova
        if(posicao_livre == -1)
        {
            return false; //Sem lugar
        }

        item = &hash_table->hash[posicao_livre];
        if(!create_hash_item(item, word))
        {
            return false;
        }
        hash_table->diff_words++;
    }
    // empate: vale a palavra que vem primeiro em ordem alfabética
    if(hash_table->most_frequent == NULL || is_more_frequent(item, hash_table->most_frequent))
    {
        hash_table->most_frequent = item;
    }

    return true;
}

extern bool remove_hash_table(HASH_TABLE *hash_table, const char *word)
{
    if(hash_table == NULL || word == NULL)
    {
        return false;
    }

    int hash_inicial = hash_code(word, hash_table->max_hash_size);
    int posicao_livre;
    int posicao = sondagem_quadratica(hash_table, word, hash_inicial, &posicao_livre);

    if(posicao == -1)
    {
        return false;
    }

    HASH_ITEM *item = &hash_table->hash[posicao];
    bool era_most_frequent = (item == hash_table->most_frequent);

    delete_hash_item(item);
    hash_table->diff_words--;

    if(era_most_frequent)
    {
        recalcular_most_frequent(hash_table);
    }

    return true;
}

extern HASH_ITEM *search_hash_table(HASH_TABLE *hash_table, const char *word)
{
    if(hash_table == NULL || word == NULL)
    {
        return NULL;
    }

    int hash_inicial = hash_code(word, hash_table->max_hash_size);
    int posicao_livre;
    int posicao = sondagem_quadratica(hash_table, word, hash_inicial, &posicao_livre);

    if(posicao == -1)
    {
        return NULL;
    }

    return &hash_table->hash[posicao];
}

extern int get_diff_words(HASH_TABLE *hash_table)
{
    return hash_table == NULL ? -1 : hash_table->diff_words;
}

extern HASH_ITEM *get_most_frequent(HASH_TABLE *hash_table)
{
    return hash_table == NULL ? NULL : hash_table->most_frequent;
}

extern void print_hash_table(HASH_TABLE *hash_table)
{
    if(hash_table == NULL)
    {
        return;
    }

    for(int i = 0; i < hash_table->max_hash_size; i++)
    {
        const HASH_ITEM *item = &hash_table->hash[i];

        //ignora as nunca usadas e as removidas
        if(!is_empty(item) && !is_removed(item))
        {
            printf("%s %d\n", get_word(item), i); // palavra e posição na tabela
        }
    }
}

/**
 * @brief Procura de novo o HASH_ITEM com mais aparições
 * Em empate, vale a palavra que vem primeiro em ordem alfabética.
 * @param[in,out] Ponteiro para HASH_TABLE
 */
static void recalcular_most_frequent(HASH_TABLE *hash_table)
{
    hash_table->most_frequent = NULL;

    for(int i = 0; i < hash_table->max_hash_size; i++)
    {
        HASH_ITEM *item = &hash_table->hash[i];

        if(is_empty(item) || is_removed(item))
        {
            continue;
        }

        if(hash_table->most_frequent == NULL || is_more_frequent(item, hash_table->most_frequent))
        {
            hash_table->most_frequent = item;
        }
    }
}

/**
 * @brief Compara dois HASH_ITEM pela frequência.
 * @param[in] HASH_ITEM candidato
 * @param[in] HASH_ITEM atual
 * @return True se item aparece mais vezes que outro ou, em empate, se a
 * palavra de item vem antes em ordem alfabética.
 */
static bool is_more_frequent(const HASH_ITEM *item, const HASH_ITEM *outro)
{
    int a = get_aparicoes(item);
    int b = get_aparicoes(outro);

    if(a != b)
    {
        return a > b;
    }

    return strcmp(get_word(item), get_word(outro)) < 0;
}

/**
 * @brief Calcula o HASHCODE de uma PALAVRA (fórmula do enunciado)
 * Para cada caractere: v += v*3 + ascii(caractere); v = v % S
 * @param[in] PALAVRA utilizada
 * @param[in] Tamanho da TABELA HASH (S)
 * @return Retorna o HASHCODE, entre 0 e hash_size - 1
 */
static int hash_code(const char *word, int hash_size)
{
    long long hash_code = 0;

    for(const char *p = word; *p != '\0'; p++)
    {
        hash_code += (hash_code * 3) + (unsigned char)*p;
        hash_code %= hash_size;
    }

    return (int)hash_code;
}

/**
 * @brief Procura a palavra seguindo a SONDAGEM QUADRÁTICA:
 *        posicao(t) = (hash_inicial + c1*t + c2*t^2) % S, t = 0, 1, 2, ...
 * @param[in] ponteiro para HASH_TABLE
 * @param[in] PALAVRA procurada
 * @param[in] hHASHCODE de word
 * @param[out] posicao_livre Primeira posição reaproveitável
 * (vazia ou removida), -1 se não houver nenhuma
 * @return Posição da palavra, se existir, ou -1 se não existir.
 */
static int sondagem_quadratica(const HASH_TABLE *hash_table, const char *word, int hash_inicial, int *posicao_livre)
{
    const HASH_ITEM *hash = hash_table->hash;
    long long hash_size = hash_table->max_hash_size;

    // constantes reduzidas
    long long c1 = ((hash_table->const_um % hash_size) + hash_size) % hash_size;
    long long c2 = ((hash_table->const_dois % hash_size) + hash_size) % hash_size;

    *posicao_livre = -1;

    for(long long t = 0; t < hash_size; t++)
    {
        long long aux = hash_inicial + ((c1 * t) % hash_size) + ((c2 * ((t * t) % hash_size)) % hash_size);
        int posicao = (int)(aux % hash_size);
        const HASH_ITEM *item = &hash[posicao];

        if(is_empty(item))
        {
            // posição nunca usada
            if(*posicao_livre == -1)
            {
                *posicao_livre = posicao;
            }
            return -1;
        }

        if(is_removed(item))
        {
            if(*posicao_livre == -1)
            {
                *posicao_livre = posicao;
            }
            continue;
        }

        if(strcmp(get_word(item), word) == 0)
        {
            return posicao;
        }
    }

    return -1;
}
