#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "sim.h"


struct sim
{
    int campo;
    double alvo;
    double tolerancia;
    double peso;
};

extern SIM *create_sim(int campo, double alvo, double tolerancia, double peso)
{
    SIM *sim = calloc(1, sizeof(SIM));
    if(sim == NULL)
    {
        return NULL;
    }

    sim->campo = campo;
    sim->alvo = alvo;
    sim->tolerancia = tolerancia;
    sim->peso = peso;

    return sim;
}
extern bool delete_sim(SIM **sim)
{
    if(sim == NULL || *sim == NULL)
    {
        return false;
    }

    free(*sim);
    *sim = NULL;

    return true;
}

extern double calculate_similarity(SIM *sim, double x)
{
    double q = sim->alvo;
    double t = sim->tolerancia;
    double result = fmax(0.0, 1 - (fabs(x-q)/t));

    return result;
}

extern double total_similarity(SIM *const sim[], int total_sim, const ITEM *item)
{
    // A similaridade total é a média ponderada dos critérios SIM
    if (sim == NULL || total_sim <= 0 || item == NULL)
    {
        return -1;
    }

    double soma = 0;
    double total_peso = 0;

    for (int i = 0; i < total_sim; i++)
    {
        double valor_sim = 0;
        const void *conteudo = get_field_content(item, sim[i]->campo);

        if (conteudo != NULL)
        {
            DATATYPE tipo = item_get_type(item, sim[i]->campo);
            if (tipo == ITEM_INT)
            {
                valor_sim = calculate_similarity(sim[i], (double)*(const int *)conteudo);
            }
            else if (tipo == ITEM_DOUBLE)
            {
                valor_sim = calculate_similarity(sim[i], *(const double *)conteudo);
            }
        }

        soma += valor_sim * sim[i]->peso;
        total_peso += sim[i]->peso;
    }

    return (total_peso > 0) ? soma / total_peso : -1;
}

extern int get_sim_campo(const SIM *sim)
{
    return (sim == NULL) ? -1 : sim->campo;
}
extern double get_sim_alvo(const SIM *sim)
{
    return (sim == NULL) ? NAN : sim->alvo;
}
extern double get_sim_tolerancia(const SIM *sim)
{
    return (sim == NULL) ? NAN : sim->tolerancia;
}
extern double get_sim_peso(const SIM *sim)
{
    return (sim == NULL) ? NAN : sim->peso;
}
