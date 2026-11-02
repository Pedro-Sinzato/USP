#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "query.h"
#include "sim.h"
#include "filter.h"
#include "../nasa.h"
#include "../descricao_item/item_reader.h"

// FILTER: condição obrigatória para o registro ser aceito.
// SIM: critério numérico de similaridade, no formato campo|alvo|tolerancia|peso;
// ACCEPT: limiar mínimo para a similaridade total;
// ORDER: chave usada pela lista ordenada;
// GROUP: campos usados pela lista generalizada;
// CROSS: campos usados pela lista cruzada.

#define MAX_SIMS 10
#define MAX_FILTERS 10
#define MAX_GROUP 4

struct query
{
    /* FILTER */
    FILTER *filters[MAX_FILTERS];
    int n_filters;
    /* SIM */
    SIM *sims[MAX_SIMS];
    int n_sims;
    /* ACCEPT */
    bool has_accept;
    double accept;     // limiar mínimo da similaridade total
    /* CROSS */
    bool  has_cross;
    int cross_fields_pos[2];
    ORDEM cross_dir[2];
    /* GROUP */
    int group[MAX_GROUP];   // níveis da lista generalizada (do mais externo ao mais interno)
    int   n_group;
    /* ORDER */
    bool  has_order;
    bool  order_by_sim;   // true: ORDER|similarity|...; false: usa order_campo
    int order_field_pos;
    ORDEM order_dir;
};

static QUERY *create_query(void)
{
    QUERY *query = calloc(1, sizeof(QUERY));
    if(query == NULL)
    {
        return NULL;
    }
    return query;
}
extern bool delete_query(QUERY *query)
{
    if (query == NULL)
        return false;

    for (int i = 0; i < query->n_sims; i++)
    {
        delete_sim(&query->sims[i]);
    }

    for (int i = 0; i < query->n_filters; i++)
    {
        delete_filter(&query->filters[i]);
    }

    free(query);
    return true;
}

static bool add_sim(QUERY *query, int campo, double alvo, double tol, double peso)
{
    if (query->n_sims >= MAX_SIMS) return false;
    SIM *sim = create_sim(campo, alvo, tol, peso);
    if (sim == NULL) return false;
    query->sims[query->n_sims++] = sim;
    return true;
}

static bool add_filter(QUERY *query, int campo,
                       const char *operador, const char *valor)
{
    if (query == NULL || query->n_filters >= MAX_FILTERS)
        return false;

    FILTER *filter = create_filter(campo, operador, valor);

    if (filter == NULL)
        return false;

    query->filters[query->n_filters++] = filter;

    return true;
}

// Lê até MAX_GROUP campos da linha atual.
// Retorna a quantidade lida, ou -1 se algum campo for inválido.
static int read_campos(int *destino, const C_NOMES *c_nomes)
{
    int n_campos = 0;
    char *token;

    while ((token = strtok(NULL, "|")) != NULL)
    {
        if (n_campos >= MAX_GROUP)
            return -1;

        int campo = name_pos(c_nomes, token);

        if (campo < 0)
            return -1;

        destino[n_campos++] = campo;
    }

    return n_campos;
}
QUERY *query_reader(const char *query_name, C_NOMES* c_nomes)
{
    FILE *query_file = fopen(query_name, "r");
    if (query_file == NULL)
    {
        return NULL;
    }

    QUERY *query = create_query();
    if (query == NULL)
    {
        fclose(query_file);
        return NULL;
    }

    char line[128];
    while (fgets(line, sizeof(line), query_file) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';

        char *tipo = strtok(line, "|");
        if (tipo == NULL)
        {
            continue; // linha vazia
        }
        if (strcmp(tipo, "SIM") == 0)
        {
            char *s_campo = strtok(NULL, "|");
            char *s_alvo  = strtok(NULL, "|");
            char *s_tol   = strtok(NULL, "|");
            char *s_peso  = strtok(NULL, "|");
            if (!s_campo || !s_alvo || !s_tol || !s_peso) continue;

            int campo = name_pos(c_nomes, s_campo);
            if (campo < 0) continue;

            add_sim(query,campo, strtod(s_alvo, NULL),strtod(s_tol, NULL), strtod(s_peso, NULL));
        }
        else if (strcmp(tipo, "ACCEPT") == 0)
        {
            char *s_limiar = strtok(NULL, "|");
            if (!s_limiar) continue;

            char *fim;
            double limiar = strtod(s_limiar, &fim);
            if (fim == s_limiar) continue;     // não era um número
            query->accept = limiar;
            query->has_accept = true;
        }
        else if (strcmp(tipo, "ORDER") == 0)
        {
            char *s_campo = strtok(NULL, "|");
            char *s_direcao   = strtok(NULL, "|");
            if (s_campo == NULL ||  s_direcao == NULL)
            {
                continue; //Linha inválida
            }

            ORDEM direcao;
            if (strcmp(s_direcao, "ASCE") == 0)
            {
                direcao = ORDEM_ASC;
            }
            else if (strcmp(s_direcao, "DESC") == 0)
            {
                direcao = ORDEM_DESC;
            }
            else
            {
                continue; // direção inválida
            }

            if (strcmp(s_campo, "similarity") == 0)
            {
                query->order_by_sim = true;
            }
            else
            {
                int campo = name_pos(c_nomes, s_campo);
                if (campo < 0)
                {
                    continue;   // campo inválido
                }
                query->order_by_sim = false;
                query->order_field_pos = campo;
            }

            query->order_dir = direcao;
            query->has_order = true;
        }
        else if (strcmp(tipo, "GROUP") == 0)
        {
            int n_campos = read_campos(query->group, c_nomes);
            query->n_group = (n_campos < 0) ? 0 : n_campos; //Se algum campo for inválido, a linha inteira é ignorada
        }
        else if (strcmp(tipo, "CROSS") == 0)
        {
            char *s_campo1 = strtok(NULL, "|");
            char *s_campo2 = strtok(NULL, "|");
            char *s_direcao1  = strtok(NULL, "|");
            char *s_direcao2  = strtok(NULL, "|");
            if (!s_campo1 || !s_campo2 ||!s_direcao1 || !s_direcao2)
            {
                continue; //linha vazia
            }
            int campo1 = name_pos(c_nomes, s_campo1);
            int campo2 = name_pos(c_nomes, s_campo2);
            if (campo1 < 0 || campo2 < 0)
            {
                 continue; //Campos inválidos
            }
            
            ORDEM direcao1;
            if (strcmp(s_direcao1, "ASCE") == 0)
            {
                direcao1 = ORDEM_ASC;
            }
            else if (strcmp(s_direcao1, "DESC") == 0)
            {
                direcao1 = ORDEM_DESC;
            }
            else
            {
                continue; // direção inválida
            }
            ORDEM direcao2;
            if (strcmp(s_direcao2, "ASCE") == 0)
            {
                direcao2 = ORDEM_ASC;
            }
            else if (strcmp(s_direcao2, "DESC") == 0)
            {
                direcao2 = ORDEM_DESC;
            }
            else
            {
                continue; // direção inválida
            }

            query->cross_fields_pos[0] = campo1;
            query->cross_fields_pos[1] = campo2;
            query->cross_dir[0] = direcao1;
            query->cross_dir[1] = direcao2;
            query->has_cross = true;
        }
             else if (strcmp(tipo, "FILTER") == 0)
        {
            char *s_campo = strtok(NULL, "|");
            char *s_operador = strtok(NULL, "|");
            char *s_valor = strtok(NULL, "|");

            if (s_campo == NULL || s_operador == NULL || s_valor == NULL)
                continue;

            int campo = name_pos(c_nomes, s_campo);

            if (campo < 0)
             continue;

            add_filter(query, campo, s_operador, s_valor);
        }
    }

    fclose(query_file);

    return query;
}

//Teste se a leitura funcionou
void print_query(const QUERY *query, C_NOMES* c_nomes)
{
    printf("Numero de SIMS: %d\n", query->n_sims);
    for (int i = 0; i < query->n_sims; i++)
    {
        const SIM *s = query->sims[i];
        printf("%s|%lf|%lf|%lf\n", get_name_in_pos(c_nomes, get_sim_campo(s)),get_sim_alvo(s), get_sim_tolerancia(s), get_sim_peso(s));
    }
    if (query->has_accept)
    {
        printf("ACCEPT: %lf\n", query->accept);
    }
    if (query->has_cross)
    {
        printf("CROSS: %s|%s\n", get_name_in_pos(c_nomes, query->cross_fields_pos[0]),get_name_in_pos(c_nomes, query->cross_fields_pos[1]));
    }
    if(query->n_group > 0)
    {
        printf("GROUP:");
        for (int i = 0; i < query->n_group; i++)
        {
        //ja alterei, ta safe
        printf(" %s", get_name_in_pos(c_nomes, query->group[i]));//talvez precise alterar isso pra um int e usar o get_name_in_pos
        }
        printf("\n");
    }
    if (query->has_order)
    {
        printf("ORDER: %s %s\n",query->order_by_sim ? "similarity" : get_name_in_pos(c_nomes, query->order_field_pos),query->order_dir == ORDEM_ASC ? "ASC" : "DESC");
    }
}
/* ACCEPT */
extern bool query_has_accept(const QUERY *query)
{
    return query != NULL && query->has_accept;
}
extern double query_accept(const QUERY *query)
{
    if (query == NULL || !query->has_accept)
    {
        return NAN;
    }
    return query->accept;
}
/* ORDER */
extern bool query_has_order(const QUERY *query)
{
    return (query == NULL) ? false : query->has_order;
}
extern bool query_order_by_similarity(const QUERY *query)
{
    return query != NULL && query->has_order && query->order_by_sim;
}
extern int query_order_campo(const QUERY *query)
{
    if (query == NULL || !query->has_order || query->order_by_sim)
    {
        return -1;
    }
    return query->order_field_pos;
}
extern ORDEM query_order_dir(const QUERY *query)
{
    return (query == NULL) ? ORDEM_ASC : query->order_dir;
}
/* GROUP */
extern int query_n_group(const QUERY *query)
{
    return (query == NULL) ? 0 : query->n_group;
}
extern int query_group(const QUERY *query, int i)
{
    if (query == NULL || i < 0 || i >= query->n_group)
        return -1;

    return query->group[i];
}
/* CROSS */
extern bool query_has_cross(const QUERY *query)
{
    return (query == NULL) ? false : query->has_cross;
}
extern int query_cross(const QUERY *query, int i)
{
    if (query == NULL || query->has_cross == false || i < 0 || i > 1)
    {
        return -1;
    }
    return query->cross_fields_pos[i];
}
/* FILTER */
extern int query_n_filters(const QUERY *query)
{
    return query == NULL ? 0 : query->n_filters;
}

extern const FILTER *query_get_filter(const QUERY *query, int i)
{
    if (query == NULL || i < 0 || i >= query->n_filters)
        return NULL;

    return query->filters[i];
}
// olha se o item satisfaz todas condicoes do FILTRO da consulta
extern bool apply_filters(const ITEM *item, const QUERY *query)
{
    if (item == NULL || query == NULL)
        return false;

    for (int i = 0; i < query_n_filters(query); i++)
    {
        const FILTER *filter = query_get_filter(query, i);

        if (!apply_filter(item, filter))
            return false;
    }

    return true;
}
