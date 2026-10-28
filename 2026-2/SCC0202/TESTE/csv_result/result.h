#ifndef RESULT_H
#define RESULT_H

#include <stdbool.h>

#include "../estruturas_dados/estruturas_dados.h"
#include "../leitor_argumentos/argument_reader.h"

/**
 * @brief Salva o conteúdo da estrutura num arquivo CSV.
 * O arquivo é o de -output; a primeira linha (cabeçalho) é copiada do arquivo
 * de -input; os ITEMs são gravados na ordem de percurso da estrutura e, se
 * -top-k for maior que zero, só os k primeiros. A estrutura não é alterada.
 * @param[in] ed Estrutura com os ITEMs a salvar.
 * @param[in] arg Argumentos do programa (-input, -output e -top-k).
 * @return true se conseguiu, false caso contrário (com mensagem em stderr).
 */
extern bool save_result(const ED *ed, const ARGUMENTS *arg);

#endif
