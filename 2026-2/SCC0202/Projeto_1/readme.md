RELATÓRIO PROJETO:

Perguntei pro Claude e ele fez os tipos de dados de da campo.

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
    char   threat_category[16];        // SAFE, MONITOR, CONCERN, OH_NO
    char   panic_verdict[48];          // frase de até 34 caracteres
    bool   on_sentry_list;
    double sentry_impact_prob;         // 1e-10 a 0.10 (vazio fora da Sentry)
    double sentry_torino_scale;        // no dataset só aparece 0.0
    double sentry_palermo_scale;       // negativo, -12.14 a -0.92
    double sentry_diameter_km;         // 0.001 a 1.3
} ASTEROID;
