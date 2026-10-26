#ifndef ITEM_READER_H
#define ITEM_READER_H

#include <stdbool.h>

#include "../leitor_argumentos/argument_reader.h"
#include "../estruturas_dados/item.h"

/* Nomes e tipos dos campos de um ITEM, lidos de um arquivo "nome|tipo".
 * A struct é opaca: use as funções abaixo para acessá-la. */
typedef struct campos_nomes C_NOMES;

C_NOMES *create_c_nomes(void);
bool destroy_c_nomes(C_NOMES **c_nomes);

/* Lê o arquivo indicado por get_reader(args). Cada linha: nome|tipo,
 * com tipo em {int, double, char, bool, string}.
 * Devolve false em qualquer erro (o chamador ainda deve destruir c_nomes). */
bool read_item(char *file, C_NOMES *c_nomes);

/* Tipo do campo i, ou ITEM_INVALID se c_nomes for NULL ou i for inválido. */
DATATYPE get_tipo(const C_NOMES *c_nomes, int i);

/* Posição do campo com esse nome, ou -1 se não existir. */
int name_pos(const C_NOMES *c_nomes, const char *nome);

/* Vetor de tipos e quantidade de campos, para passar ao reader. */
const DATATYPE *get_tipos(const C_NOMES *c_nomes);
int get_n_campos(const C_NOMES *c_nomes);

#endif
