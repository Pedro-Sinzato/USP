#ifndef SIM_H
#define SIM_H

#include "../nasa.h"

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
extern bool delete_sim(SIM **sim);
/**
 * @brief Calcula a SIMILARIDADE de um SIM.
 * @param[in] Ponteiro para SIM.
 * @return Valor de SIMILARIDADE calculado.
 */
extern double similarity(SIM *sim);

/**
 * @brief Calcula a SIMILARIDADE entre os parametros estabelecidos em um SIM e um valor (geralmente pego de um item).
 * @param[in] Ponteiro para SIM.
 * @param[in] Valor para ser comparado em relacao a tolerancia
 * @return Valor de SIMILARIDADE calculado.
 */
extern double calculate_similarity(SIM *sim, double x);

/**
 * @brief Lê a DATABASE.
 * @param[in] Nome do arquivo DATABASE.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern double total_similarity(SIM *sim[], int total_sim);

extern int get_sim_campo(const SIM *sim);
extern double get_sim_alvo(const SIM *sim);
extern double get_sim_tolerancia(const SIM *sim);
extern double get_sim_peso(const SIM *sim);

#endif
