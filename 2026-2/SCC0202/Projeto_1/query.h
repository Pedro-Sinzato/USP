#ifndef QUERY_H
#define QUERY_H

typedef enum campo
{
    F_YEAR, F_MONTH, F_DISTANCE_AU, F_VELOCITY_KM_S, F_ABS_MAG,
    F_DAYS_UNTIL, F_IS_PAST, F_IS_FUTURE, F_RISK_SCORE,
    F_PANIC_LEVEL, F_THREAT_CATEGORY, F_ON_SENTRY, F_SENTRY_IMPACT_PROP,
    F_SENTRY_TORINO_SCALE, F_SENTRY_PALERMO_SCALE, F_SENTRY_DIAMETER_KM, F_INVALID
} CAMPO;
typedef enum operador
{
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE, OP_INVALID
}OPERADOR;

/**
 * @brief Faz o parsing do CAMPO
 * @param[in] String contendo o campo.
 * @return ENUM CAMPO correspondente.
 */
static CAMPO parse_campo(const char *campo);
/**
 * @brief FAz o parsing do OPERADOR
 * @param[in] String contendo o operador.
 * @return ENUM OPERADOR correspondente.
 */
static OPERADOR parse_operador(const char *operador);

#endif
