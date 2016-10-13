#ifndef AVALANCHE_H_
#define AVALANCHE_H_

double avalanche_generation_rate(double electron_density, double electron_temperature,	double effective_charge, double electric_field, double magnetic_field, int modulevar_avalanche);
double calculate_flow_Ap(double p, double E, double Z);
double calculate_flow_costheta(double p, double E, double Z);
double calculate_flow_velocity(double electron_density, double effective_charge, double electric_field, double magnetic_field)
#endif /* AVALANCHE_H_ */
