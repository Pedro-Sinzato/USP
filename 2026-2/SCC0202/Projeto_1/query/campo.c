#include <string.h>

#include "campo.h"

extern CAMPO parse_campo(const char *campo)
{
    if (!strcmp(campo, "year")) return F_YEAR;
    if (!strcmp(campo, "distance_au")) return F_DISTANCE_AU;
    if (!strcmp(campo, "velocity_km_s")) return F_VELOCITY_KM_S;
    if (!strcmp(campo, "risk_score")) return F_RISK_SCORE;
    if (!strcmp(campo, "is_future_event")) return F_IS_FUTURE;
    if (!strcmp(campo, "threat_category")) return F_THREAT_CATEGORY;
    if (!strcmp(campo, "on_sentry_list")) return F_ON_SENTRY;
    if (!strcmp(campo, "sentry_impact_prob")) return F_SENTRY_IMPACT_PROP;
    if (!strcmp(campo, "sentry_torino_scale")) return F_SENTRY_TORINO_SCALE;
    if (!strcmp(campo, "sentry_palermo_scale")) return F_SENTRY_PALERMO_SCALE;
    if (!strcmp(campo, "sentry_diameter_km")) return F_SENTRY_DIAMETER_KM;
    return F_INVALID;
}
