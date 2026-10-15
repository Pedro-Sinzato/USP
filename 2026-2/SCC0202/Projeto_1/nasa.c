#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

#include "nasa.h"
#include "estruturas_dados/item.h"
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

DATATYPE asteroid_fields[30] =
{
    ITEM_PCHAR, //Asteroid_designation
    ITEM_PCHAR, //ASteroid_fullname
    ITEM_PCHAR, //Close_approach_date
    ITEM_INT, //Year
    ITEM_INT, //Month
    ITEM_DOUBLE, //Distance_au
    ITEM_DOUBLE, //Velocity_km_s
    ITEM_DOUBLE, //Absolute_magnitude
    ITEM_INT, //Days_until_approach
    ITEM_BOOL, //Is_past_event
    ITEM_BOOL, //Is_future_event
    ITEM_DOUBLE, //Risk_score
    ITEM_INT, //Panic_level
    ITEM_PCHAR, //Threat_category
    ITEM_PCHAR, //Threat_veridct
    ITEM_BOOL, //On_sentry_list
    ITEM_DOUBLE, //Sentry_impact_prob
    ITEM_DOUBLE, //Sentry_torino_scale
    ITEM_DOUBLE, //Sentry_palermo_scale
    ITEM_DOUBLE //Sentry_diameter_km
};
