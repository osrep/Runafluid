#ifndef CRITICAL_FIELD_H_
#define CRITICAL_FIELD_H_

#include "constants.h"

int is_field_critical(profile pro);

double calculate_critical_field(double electron_density, double electron_temperature);

#endif /* CRITICAL_FIELD_H_ */
