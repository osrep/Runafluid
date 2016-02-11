#ifndef AVALANCHE_RATE_H_
#define AVALANCHE_RATE_H_

#include "constants.h"

int is_field_critical(profile pro);

double calculate_critical_field(double electron_density, double electron_temperature);

#endif /* AVALANCHE_RATE_H_ */
