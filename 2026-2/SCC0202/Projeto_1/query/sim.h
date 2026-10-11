#ifndef SIM_H
#define SIM_H

#include "campo.h"

/**
 * @brief Estrutura do SIM.
 * Campo
 * Alvo
 * Tolerancia
 * Peso
 */
typedef struct sim SIM;

/**
 * @brief Cria um SIM.
 * @param[in] Campo do SIM.
 * @param[in] Alvo do SIM.
 * @param[in] Tolerancia do SIM.
 * @param[in] Peso do SIM.
 * @return Ponteiro para o SIM criado, se não conseguir, Ponteiro para NULL.
 */
extern SIM *create_sim(CAMPO campo, double alvo, double tolerancia, double peso);

/**
 * @brief Calcula a SIMILARIDADE de um SIM.
 * @param[in] Ponteiro para SIM.
 * @return Valor de SIMILARIDADE calculado.
 */
extern double similarity(SIM *sim);
/**
 * @brief Lê a DATABASE.
 * @param[in] Nome do arquivo DATABASE.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern double total_similarity(SIM *sim[], int total_sim);

#endif
