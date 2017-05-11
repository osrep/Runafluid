#ifndef DREICER_H_
#define DREICER_H_
#include "control.h"

double dreicer_generation_rate(double electron_density, double electron_temperature,
                               double effective_charge, double electric_field, int formula_id);

double calculate_toroidicity_dreicer(double inv_asp_ratio);

double calculate_alpha(double electric_field, double electron_density, double electron_temperature);

double calculate_lambda(double alpha);

double calculate_gamma(double effective_charge, double alpha);

double calculate_h(double alpha, double effective_charge);

#endif /* DREICER_H_ */
