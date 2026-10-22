#ifndef READER_H
#define READER_H

#include <stdbool.h>

#include "../estruturas_dados/estruturas_dados.h"
#include "../leitor_argumentos/argument_reader.h"
#include "../estruturas_dados/item.h"

/**
 * @brief Lê o CSV de -input (pulando o cabeçalho) e insere um ITEM por linha
 * na estrutura, usando insert_ed. Se -limit for maior que zero, lê no máximo
 * essa quantidade de registros; 0 significa todos.
 * @param[in,out] ed Estrutura que recebe os ITEMs.
 * @param[in] arg Argumentos do programa (-input e -limit).
 * @param[in] tipos Tipo de cada campo da linha.
 * @param[in] n_campos Quantidade de campos de cada linha.
 * @return true se leu sem erro, false caso contrário.
 */
bool reader(ED *ed, ARGUMENTS *arg, const DATATYPE tipos[], int n_campos);

#endif
