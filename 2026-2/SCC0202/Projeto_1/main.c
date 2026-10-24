#include <stdlib.h>
#include <stdbool.h>

#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/estruturas_dados.h"
#include "csv_reader/reader.h"
#include "camposNasa.h"

static const DATATYPE TIPOS_ASTEROIDE[NASA_N_CAMPOS] = {
    [NASA_DESIGNACAO]           = ITEM_STRING,
    [NASA_NOME_COMPLETO]        = ITEM_STRING,
    [NASA_DATA_APROXIMACAO]     = ITEM_STRING,
    [NASA_CATEGORIA_AMEACA]     = ITEM_STRING,
    [NASA_PANIC_VERDICT]        = ITEM_STRING,
    [NASA_EVENTO_PASSADO]       = ITEM_BOOL,
    [NASA_EVENTO_FUTURO]        = ITEM_BOOL,
    [NASA_NA_LISTA_SENTRY]      = ITEM_BOOL,
    [NASA_ANO]                  = ITEM_INT,
    [NASA_MES]                  = ITEM_INT,
    [NASA_DIAS_ATE_APROXIMACAO] = ITEM_INT,
    [NASA_PANIC_LEVEL]          = ITEM_INT,
    [NASA_DISTANCIA_AU]         = ITEM_DOUBLE,
    [NASA_VELOCIDADE_KM_S]      = ITEM_DOUBLE,
    [NASA_MAGNITUDE_ABSOLUTA]   = ITEM_DOUBLE,
    [NASA_RISK_SCORE]           = ITEM_DOUBLE,
    [NASA_SENTRY_IMPACT_PROB]   = ITEM_DOUBLE,
    [NASA_SENTRY_TORINO]        = ITEM_DOUBLE,
    [NASA_SENTRY_PALERMO]       = ITEM_DOUBLE,
    [NASA_SENTRY_DIAMETRO_KM]   = ITEM_DOUBLE,
};

int main(int argc, char *argv[])
{
    ARGUMENTS *arg = create_arguments();
    argument_parser(argc, argv, arg);

    ED *ed  = create_ed(ed_type_from_name(get_base(arg)));

    reader(ed, arg, TIPOS_ASTEROIDE, NASA_N_CAMPOS);
    print_ed(ed);

    delete_arguments(&arg);
    destroy_ed(&ed);                          /* libera os itens da fila */

    return EXIT_SUCCESS;
}
