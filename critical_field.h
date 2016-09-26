#ifndef CRITICAL_FIELD_H_
#define CRITICAL_FIELD_H_

#include "control.h"

int is_field_critical(profile pro);

double calculate_critical_field(double electron_density, double electron_temperature);

double calculate_dreicer_field(double electron_density, double electron_temperature);

double calculate_dreicer_field2(double electron_density, double electron_temperature);

double calculate_coulomb_log(double electron_density, double electron_temperature);

double calculate_thermal_electron_collision_time(double electron_density, double coulomb_log);


#endif /* CRITICAL_FIELD_H_ */
