#ifndef DREICER_H_
#define DREICER_H_

#include "control.h"

double dreicer_generation_rate(double electron_density, double electron_temperature, double effective_charge, double electric_field, int formula_id);

#endif /* DREICER_H_ */
