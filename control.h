#ifndef CONTROL_H_
#define CONTROL_H_

//! cell structure	
#include <vector>	
struct cell {
	double rho;
	double electron_density;
	double electron_temperature;
	double effective_charge;
	double electric_field;
	double magnetic_field;
	double runaway_density; 
};

typedef std::vector<cell> profile;

double runafluid_control(double electron_density, double rundensity_before, double electron_temperature,
		double effective_charge, double electric_field, double magnetic_field, double timestep, int runafluid_switch, double *rate_values);
		
		
#endif /* CONTROL_H_ */
