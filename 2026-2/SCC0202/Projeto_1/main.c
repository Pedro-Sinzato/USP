#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "descricao_item/item_reader.h"
#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/estruturas_dados.h"
#include "query/query.h"
#include "csv_reader/reader.h"
#include "csv_result/result.h"
#include "busca/search.h"

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
    unsigned long long input_records;
    unsigned long long matched_records;
    unsigned long long output_records;
    double load_ms;
    double search_ms;
    double result_insert_ms;
    double write_ms;
} STATUS;

static void print_status(const STATUS *st)
{
    printf("STATUS=%s\n", st->status ? "OK" : "ERROR");
    printf("INPUT_RECORDS=%llu\n", st->input_records);
    printf("MATCHED_RECORDS=%llu\n", st->matched_records);
    printf("OUTPUT_RECORDS=%llu\n", st->output_records);
    printf("LOAD_MS=%.3f\n", st->load_ms);
    printf("SEARCH_MS=%.3f\n", st->search_ms);
    printf("RESULT_INSERT_MS=%.3f\n", st->result_insert_ms);
    printf("WRITE_MS=%.3f\n", st->write_ms);
}

int main(int argc, char *argv[])
{
    STATUS st = {0};

    ARGUMENTS *arg = create_arguments();
    C_NOMES *c_nomes = create_c_nomes();
    QUERY *query = NULL;
    ED *base = NULL;
    ED *result = NULL;
    ITEM **found = NULL;
    size_t n_found = 0;
    size_t n_scanned = 0;
    bool status = true;

    if (arg == NULL || c_nomes == NULL)
    {
        fprintf(stderr, "falta de memoria\n");
        goto fim;
    }

    if (argument_parser(argc, argv, arg) == false)
    {
        fprintf(stderr, "uso: %s --reader R --input arq.csv --base B --result R --query q.txt --output saida.csv [--top-k K] [--limit L]\n", argv[0]);
        goto fim;
    }
    if (read_item(get_reader(arg), c_nomes) == false)
    {
        fprintf(stderr, "nao foi possivel ler a descricao dos campos '%s'\n",
                get_reader(arg));
        goto fim;
    }

    ED_TYPES base_type = ed_type_from_name(get_base(arg));
    ED_TYPES result_type = ed_type_from_name(get_result(arg));
    if (base_type == ED_INVALID || result_type == ED_INVALID)
    {
        fprintf(stderr, "estrutura desconhecida em --base ou --result\n");
        goto fim;
    }

    query = query_reader(get_query(arg), c_nomes);
    if (query == NULL)
    {
        fprintf(stderr, "nao foi possivel ler a query '%s'\n", get_query(arg));
        goto fim;
    }

    base = create_ed(base_type, NULL);
    result = create_ed(result_type, query);
    if (base == NULL || result == NULL)
    {
        fprintf(stderr, "nao foi possivel criar as estruturas (a query tem a secao necessaria para --result %s?)\n", get_result(arg));
        goto fim;
    }

    /* 1. carga */
    MEDIR_MS(st.load_ms, status, reader(base, arg, get_tipos(c_nomes), get_n_campos(c_nomes)));
    if (!status)
    {
        fprintf(stderr, "erro ao ler '%s'\n", get_input(arg));
        goto fim;
    }

    /* 2. busca percorrendo a base */
    MEDIR_MS(st.search_ms, status, search_ed(base, query, &found, &n_found, &n_scanned));
    st.input_records = n_scanned;
    st.matched_records = n_found;
    if (!status)
    {
        fprintf(stderr, "erro durante a busca\n");
        goto fim;
    }

    /* 3. inserção dos aceitos no resultado */
    MEDIR_MS(st.result_insert_ms, status, insert_found(result, found, n_found));
    if (!status)
    {
        fprintf(stderr, "erro ao inserir no resultado\n");
        goto fim;
    }

    /* 4. gravação dos top-k */
    MEDIR_MS(st.write_ms, status, save_result(result, arg));
    if (status == false)
    {
        goto fim;
    }

    unsigned long long top_k = get_top_k(arg);
    st.output_records = (top_k == 0 || st.matched_records < top_k) ? st.matched_records : top_k;
    st.status = true;

fim:
    free(found);
    destroy_ed(&result);
    destroy_ed(&base);
    if (query != NULL)
    {
        delete_query(query);
    }
    destroy_c_nomes(&c_nomes);
    delete_arguments(&arg);

    print_status(&st);

    return st.status ? EXIT_SUCCESS : EXIT_FAILURE;
}
