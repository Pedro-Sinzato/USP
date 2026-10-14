#ifndef NASA_H
#define NASA_H

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
 * \brief Estrutura para representar um ASTEROIDE.
 *  Guarda os campos de ASTEROIDE.
 */
typedef struct asteroid ASTEROID;

/**
 * @brief Cria ASTEROID
 * @return Ponteiro para ASTEROID, se não conseguir, NULL.
 */
ASTEROID *create_asteroid(void);

/**
 * @brief Lê a DATABASE.
 * @param[in] Nome do arquivo DATABASE.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool read_database(char database_name[]);

extern CAMPO parse_campo(const char *campo);
extern const char *campo_nome(CAMPO campo);
#endif
