#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "descricao_item/item_reader.h"
#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/estruturas_dados.h"
#include "csv_reader/reader.h"

//Impreciso e não muito confiável, mas é o que tem para C99
#define MEDIR_MS(tempo_ms, resultado, expr)                                       \
        do {                                                                      \
            clock_t ini_ = clock();                                               \
            (resultado) = (expr);                                                 \
            clock_t fim_ = clock();                                               \
            (tempo_ms) = 1000.0 * (double)(fim_ - ini_) / (double)CLOCKS_PER_SEC; \
        } while (0)

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
    C_NOMES *c_nomes = create_c_nomes();

    argument_parser(argc, argv, arg);

    read_item(get_reader(arg), c_nomes);
    ED *ed  = create_ed(ed_type_from_name(get_base(arg)));

    MEDIR_MS(st.load_ms, st.status,reader(ed, arg, get_tipos(c_nomes), get_n_campos(c_nomes)));
    print_ed(ed);

    printf("STATUS\n LOAD_MS: %ld", st.load_ms);

    destroy_c_nomes(&c_nomes);
    delete_arguments(&arg);
    destroy_ed(&ed);

    return EXIT_SUCCESS;
}
