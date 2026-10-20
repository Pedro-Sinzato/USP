#include <stdlib.h>
#include <stdbool.h>

#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/stack/stack.h"
#include "reader.h"

enum nasa_campo
{
    NASA_DESIGNACAO,
    NASA_NOME_COMPLETO,
    NASA_DATA_APROXIMACAO,
    NASA_ANO,
    NASA_MES,
    NASA_DISTANCIA_AU,
    NASA_VELOCIDADE_KM_S,
    NASA_MAGNITUDE_ABSOLUTA,
    NASA_DIAS_ATE_APROXIMACAO,
    NASA_EVENTO_PASSADO,
    NASA_EVENTO_FUTURO,
    NASA_RISK_SCORE,
    NASA_PANIC_LEVEL,
    NASA_CATEGORIA_AMEACA,
    NASA_PANIC_VERDICT,
    NASA_NA_LISTA_SENTRY,
    NASA_SENTRY_IMPACT_PROB,
    NASA_SENTRY_TORINO,
    NASA_SENTRY_PALERMO,
    NASA_SENTRY_DIAMETRO_KM,
    NASA_N_CAMPOS
};
static const DATATYPE TIPOS_ASTEROIDE[NASA_N_CAMPOS] = {
    [NASA_DESIGNACAO]           = ITEM_STRING,
    [NASA_NOME_COMPLETO]        = ITEM_STRING,
    [NASA_DATA_APROXIMACAO]     = ITEM_STRING,
    [NASA_ANO]                  = ITEM_INT,
    [NASA_MES]                  = ITEM_INT,
    [NASA_DISTANCIA_AU]         = ITEM_DOUBLE,
    [NASA_VELOCIDADE_KM_S]      = ITEM_DOUBLE,
    [NASA_MAGNITUDE_ABSOLUTA]   = ITEM_DOUBLE,
    [NASA_DIAS_ATE_APROXIMACAO] = ITEM_INT,
    [NASA_EVENTO_PASSADO]       = ITEM_BOOL,
    [NASA_EVENTO_FUTURO]        = ITEM_BOOL,
    [NASA_RISK_SCORE]           = ITEM_DOUBLE,
    [NASA_PANIC_LEVEL]          = ITEM_INT,
    [NASA_CATEGORIA_AMEACA]     = ITEM_STRING,
    [NASA_PANIC_VERDICT]        = ITEM_STRING,
    [NASA_NA_LISTA_SENTRY]      = ITEM_BOOL,
    [NASA_SENTRY_IMPACT_PROB]   = ITEM_DOUBLE,
    [NASA_SENTRY_TORINO]        = ITEM_DOUBLE,
    [NASA_SENTRY_PALERMO]       = ITEM_DOUBLE,
    [NASA_SENTRY_DIAMETRO_KM]   = ITEM_DOUBLE,
};

int main(int argc, char *argv[])
{
    ARGUMENTS *arg = create_arguments();
    argument_parser(argc, argv, arg);

    STACK *stack = create_stack();

    reader(stack, arg, *TIPOS_ASTEROIDE, NASA_N_CAMPOS);

    return EXIT_SUCCESS;
}
