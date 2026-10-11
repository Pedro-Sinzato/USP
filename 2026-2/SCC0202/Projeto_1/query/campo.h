#ifndef CAMPO_H
#define CAMPO_H

/**
 * @brief ENUM com os CAMPOS possíveis.
 */
typedef enum campo
{
    F_YEAR, F_MONTH, F_DISTANCE_AU, F_VELOCITY_KM_S, F_ABS_MAG,
    F_DAYS_UNTIL, F_IS_PAST, F_IS_FUTURE, F_RISK_SCORE,
    F_PANIC_LEVEL, F_THREAT_CATEGORY, F_ON_SENTRY, F_SENTRY_IMPACT_PROP,
    F_SENTRY_TORINO_SCALE, F_SENTRY_PALERMO_SCALE, F_SENTRY_DIAMETER_KM, F_INVALID
}CAMPO;

/**
 * @brief Faz o parsing do CAMPO.
 * @param[in] String contendo o CAMPO.
 * @return Retorna o ENUM CAMPO correspondente.
 */
extern CAMPO parse_campo(const char *campo);

#endif
