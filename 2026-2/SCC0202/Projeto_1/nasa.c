#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

#include "nasa.h"
#include "item.h"
//Incluir estruturas_dados.h

typedef struct sentry_info
{
} SENTRY_INFO;

static const char *nomes[] =
{
    [F_YEAR] = "year",
    [F_MONTH] = "month",
    [F_DISTANCE_AU] = "distance_au",
    [F_VELOCITY_KM_S] = "velocity_km_s",
    [F_ABS_MAG] = "absolute_magnitude",
    [F_DAYS_UNTIL] = "days_until_approach",
    [F_IS_PAST] = "is_past_event",
    [F_IS_FUTURE] = "is_future_event",
    [F_RISK_SCORE] = "risk_score",
    [F_PANIC_LEVEL] = "panic_level",
    [F_THREAT_CATEGORY] = "threat_category",
    [F_ON_SENTRY] = "on_sentry_list",
    [F_SENTRY_IMPACT_PROP] = "sentry_impact_prob",
    [F_SENTRY_TORINO_SCALE] = "sentry_torino_scale",
    [F_SENTRY_PALERMO_SCALE] = "sentry_palermo_scale",
    [F_SENTRY_DIAMETER_KM] = "sentry_diameter_km"
};

//days_until_approach: is_past_event é True quando os dias são negativos.
struct asteroid
{
    char   asteroid_designation[16];   // ex.: "2020 AY1" (máx. 10 caracteres)
    char   asteroid_fullname[48];      // ex.: "(2020 AY1)" (máx. 34)
    char   close_approach_date[20];    // "2020-01-01 00:54:00" (19 + '\0')
    int    year;                       // 2020 a 2100
    int    month;                      // 1 a 12
    double distance_au;                // 4.5e-05 a 0.2
    double velocity_km_s;              // 0.07 a 63.4
    double absolute_magnitude;         // 10.39 a 32.95 (44 valores vazios)
    int    days_until_approach;        // -2119 a 27464 (pode ser negativo)
    bool   is_past_event;
    bool   is_future_event;
    double risk_score;                 // 0.0018 a 0.876
    int    panic_level;                // 0 a 9
    char threat_category[15];
    char   panic_verdict[48];          // frase de até 34 caracteres
    bool   on_sentry_list;
    double sentry_impact_prob;
    double sentry_torino_scale;
    double sentry_palermo_scale;
    double sentry_diameter_km;
};
