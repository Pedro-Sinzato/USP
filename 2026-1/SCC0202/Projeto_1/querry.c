#include <stdio.h>
#include <string.h>
#include <math.h>
// FILTER: condição obrigatória para o registro ser aceito;
//IM: critério numérico de similaridade, no formato campo|alvo|tolerancia|peso;
//ACCEPT: limiar mínimo para a similaridade total;
//ORDER: chave usada pela lista ordenada;
//GROUP: campos usados pela lista generalizada;
//CROSS: campos usados pela lista cruzada.

typedef struct sim
{
    double campo; //Associar campos a valores com enum?
    double alvo;
    double tolerancia;
    double peso;
}SIM;

double similarity(SIM sim)
{
    int x = 0; //Não sei de onde vem esse x
    double q = sim.alvo;
    double t = sim.tolerancia;
    double result = fmax(0.0, 1 - (fabs(x-q)/t));

    return result;
}

double total_similarity(SIM *sim, int total_sim)
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
        total_similarity += similarity(sim[i]) * sim[i].peso;
        total_peso+= sim[i].peso;
    }

    double media_ponderada = total_similarity / total_peso;

    return media_ponderada;
}
