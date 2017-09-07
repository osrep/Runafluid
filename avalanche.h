#ifndef AVALANCHE_H_
#define AVALANCHE_H_

double avalanche_generation_rate(double electron_density, double electron_temperature,	double effective_charge, double electric_field, double magnetic_field, int modulevar_avalanche);
double calculate_toroidicity_avalanche(double inv_asp_ratio, double electric_field, double electron_density, double electron_temperature, double rho_tor_norm);
double calculate_avalanche_threshold_field(double electron_density, double electron_temperature, double effective_charge, double critical_field, double magnetic_field);

#endif /* AVALANCHE_H_ */

/*double calculate_flow_Ap(double p, double E, double Z);
double calculate_flow_costheta(double p, double E, double Z);
double calculate_flow_velocity(double electron_density, double electron_temperature, double effective_charge, double electric_field, double magnetic_field);*/
