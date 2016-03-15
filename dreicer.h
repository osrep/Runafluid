#ifndef DREICER_H_
#define DREICER_H_

#include "constants.h"

int is_field_critical(profile pro);
int step_counter(profile pro);
int init_dreicer(profile pro);
double calculate_critical_field(double electron_density, double electron_temperature);
double dreicer_generation_rate(double electron_density, double electron_temperature, double effective_charge, double electric_field);

#endif /* DREICER_H_ */
