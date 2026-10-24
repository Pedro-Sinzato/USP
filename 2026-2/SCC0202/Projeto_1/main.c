#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/estruturas_dados.h"
#include "csv_reader/reader.h"
#include "camposNasa.h"

//Impreciso e não muito confiável, mas é o que tem para C99
#define MEDIR_MS(tempo_ms, resultado, expr)                                       \
        do {                                                                      \
            clock_t ini_ = clock();                                               \
            (resultado) = (expr);                                                 \
            clock_t fim_ = clock();                                               \
            (tempo_ms) = 1000.0 * (double)(fim_ - ini_) / (double)CLOCKS_PER_SEC; \
        } while (0)

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

typedef struct status
{
    bool status;
    int input_records;
    int matched_records;
    clock_t load_ms;
    clock_t search_ms;
    clock_t result_insert_ms;
    clock_t write_ms;
}STATUS;

int main(int argc, char *argv[])
{
    STATUS st;
    ARGUMENTS *arg = create_arguments();
    argument_parser(argc, argv, arg);

    ED *ed  = create_ed(ed_type_from_name(get_base(arg)));

    MEDIR_MS(st.load_ms, st.status,reader(ed, arg, TIPOS_ASTEROIDE, NASA_N_CAMPOS));
    print_ed(ed);

    printf("STATUS\n LOAD_MS: %ld", st.load_ms);

    delete_arguments(&arg);
    destroy_ed(&ed);                          /* libera os itens da fila */

    return EXIT_SUCCESS;
}
