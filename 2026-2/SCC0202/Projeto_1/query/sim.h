#ifndef SIM_H
#define SIM_H

typedef struct sim SIM;

extern SIM *add_sim(CAMPO campo, double alvo, double tolerancia, double peso);

#endif
