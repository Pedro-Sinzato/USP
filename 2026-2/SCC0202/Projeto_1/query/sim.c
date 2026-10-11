#include <stdlib.h>

#include "sim.h"
#include "campo.h"

struct sim
{
    CAMPO campo;
    double alvo;
    double tolerancia;
    double peso;
};

extern SIM *add_sim(CAMPO campo, double alvo, double tolerancia, double peso)
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
