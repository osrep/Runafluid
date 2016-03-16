#ifndef AVALANCHE_RATE_H_
#define AVALANCHE_RATE_H_

#include "constants.h"

int is_field_critical(profile pro);
int init_avalanche(profile pro, double Ea);
double calculate_critical_field(double electron_density, double electron_temperature);
double avalanche_generation_rate(double electron_density, double electron_temperature,	double effective_charge, double electric_field, double Ea);

#endif /* AVALANCHE_RATE_H_ */
