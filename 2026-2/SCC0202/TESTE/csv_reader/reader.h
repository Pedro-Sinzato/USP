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
 *
 * Formato aceito: campos separados por vírgula; campos de texto podem vir
 * entre aspas (com "" para uma aspa literal); espaços antes do campo são
 * ignorados; campo vazio numérico vira 0; linhas em branco são puladas.
 * Cada linha deve ter exatamente n_campos campos e no máximo 1023 caracteres.
 * Em caso de erro, escreve em stderr o número da linha e retorna false
 * (os ITEMs já inseridos continuam em ed; quem chamou deve destruí-la).
 *
 * @param[in,out] ed Estrutura que recebe os ITEMs.
 * @param[in] arg Argumentos do programa (-input e -limit).
 * @param[in] tipos Tipo de cada campo da linha (ex.: get_tipos do C_NOMES).
 * @param[in] n_campos Quantidade de campos de cada linha (ex.: get_n_campos).
 * @return true se leu sem erro, false caso contrário.
 */
bool reader(ED *ed, ARGUMENTS *arg, const DATATYPE tipos[], int n_campos);

#endif
