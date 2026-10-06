#include <stdbool.h>

typedef enum threat_category {
    THREAT_SAFE,
    THREAT_MONITOR,
    THREAT_CONCERN,
    THREAT_OH_NO
} THREAT_CATEGORY;

typedef struct sentry_info
{
    double sentry_impact_prob;
    double sentry_torino_scale;
    double sentry_palermo_scale;
    double sentry_diameter_km;
} SENTRY_INFO;

//days_until_approach: is_past_event é True quando os dias são negativos.
typedef struct asteroid {
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
    THREAT_CATEGORY threat_category;
    char   panic_verdict[48];          // frase de até 34 caracteres
    bool   on_sentry_list;
    SENTRY_INFO *sentry_info;
} ASTEROID;
