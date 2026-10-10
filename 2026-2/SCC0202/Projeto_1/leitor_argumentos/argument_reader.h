#ifndef ARGUMENT_READER_H
#define ARGUMENT_READER_H

/**
 * \brief Estrutura para guardar os ARGUMENTOS.
 *  Guarda os campos dos ARGUMENTOS.
 * -reader "leitor_dataset.c"
 * -input "arquivo_entrada.csv"
 * -base "estrutura_dados_entrada"
 * -result "estrutura_dados_saida"
 * -query "arquivo_criterios_busca.txt"
 * -output "arquivo_saida.csv"
 * -top-k maximo_registros_esportados; 0 = Todos
 * -limit maximo_registros carregados; 0 = Todos
 */
typedef struct arguments ARGUMENTS;

/**
 * @brief Cria ARGUMENTS.
 * @return Retorna um pontero para ARGUMENTS.
 */
extern ARGUMENTS *create_arguments(void);

/**
 * @brief Deleta ARGUMENTS.
 * @param[out] Ponteiro duplo para ARGUMENTS.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool delete_arguments(ARGUMENTS **arguments);

/**
 * @brief Copia uma palavra de ARG_V para DESTINO.
 * @param[out] Ponteiro para DESTINO.
 * @param[in] Ponteiro para a palavra em ARG_V
 */
static void copy_arg(char *destino, const char *arg_word);
/**
 * @brief Lê ARG_V,faz o parsing e guarda em ARGUMENTS.
 * @param[in] ARG_C
 * @param[in] ARG_V
 * @param[in] Ponteiro para ARGUMENTS
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool argument_parser(int argc, char *argv[], ARGUMENTS *arguments);

/**
 * @brief Devolve um dos campos de ARGUMENTS.
 * @param[in] Ponteiro para ARGUMENTS
 * @return Campo de ARGUMENTS.
 */
extern const char *get_reader(const ARGUMENTS *arguments);
extern const char *get_input(const ARGUMENTS *arguments);
extern const char *get_base(const ARGUMENTS *arguments);
extern const char *get_result(const ARGUMENTS *arguments);
extern const char *get_query(const ARGUMENTS *arguments);
extern const char *get_output(const ARGUMENTS *arguments);
extern unsigned long long int get_top_k(const ARGUMENTS *arguments);
extern unsigned long long int get_limit(const ARGUMENTS *arguments);

#endif
