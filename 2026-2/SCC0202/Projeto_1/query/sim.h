#ifndef SIM_H
#define SIM_H

#include <stdbool.h>

#include "../estruturas_dados/item.h"

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
extern SIM *create_sim(int campo, double alvo, double tolerancia, double peso);
extern bool delete_sim(SIM **sim);
/**
 * @brief Calcula a SIMILARIDADE entre os parametros estabelecidos em um SIM e um valor (geralmente pego de um item).
 * @param[in] Ponteiro para SIM.
 * @param[in] Valor para ser comparado em relacao a tolerancia
 * @return Valor de SIMILARIDADE calculado.
 */
extern double calculate_similarity(SIM *sim, double x);

/**
 * @brief Similaridade total de um ITEM: média ponderada (pelos pesos) das
 * similaridades de cada SIM, cada uma calculada sobre o campo get_sim_campo(sim)
 * do ITEM. Campos que não sejam int ou double contam como similaridade 0.
 * @param[in] sim Vetor de SIMs.
 * @param[in] total_sim Quantidade de SIMs no vetor.
 * @param[in] item ITEM avaliado.
 * @return Valor entre 0 e 1; -1 se não houver SIM, ITEM inválido ou soma dos pesos <= 0.
 */
extern double total_similarity(SIM *const sim[], int total_sim, const ITEM *item);

/**
 * @brief Funções de GET dos campos do SIM
 * @param[in] Ponteiro para SIM.
 * @return Valor do campo solicitado
 */
extern int get_sim_campo(const SIM *sim);
extern double get_sim_alvo(const SIM *sim);
extern double get_sim_tolerancia(const SIM *sim);
extern double get_sim_peso(const SIM *sim);

#endif
