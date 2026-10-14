#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "sim.h"
#include "../nasa.h"

struct sim
{
    CAMPO campo;
    double alvo;
    double tolerancia;
    double peso;
};

extern SIM *create_sim(CAMPO campo, double alvo, double tolerancia, double peso)
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

extern double similarity(SIM *sim)
{
    int x = 0; //Não sei de onde vem esse x
    double q = sim->alvo;
    double t = sim->tolerancia;
    double result = fmax(0.0, 1 - (fabs(x-q)/t));

    return result;
}

extern double total_similarity(SIM *sim[], int total_sim)
{
    //A similaridade total será a média ponderada dos critérios SIM
    double total_similarity = 0;
    double total_peso = 0;

    if(sim == NULL)
    {
        //Não há SIM
        return -1;
    }
    for(int i  = 0; i < total_sim; i++)
    {
        total_similarity += similarity(sim[i]) * sim[i]->peso;
        total_peso+= sim[i]->peso;
    }

    double media_ponderada = total_similarity / total_peso;

    return media_ponderada;
}

extern CAMPO get_sim_campo(const SIM *sim)
{
    return (sim == NULL) ? F_INVALID : sim->campo;
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
