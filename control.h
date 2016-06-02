#ifndef CONTROL_H_
#define CONTROL_H_

//#include "cpo_utils.h"

//! cell structure		
struct cell {
	double electron_density;
	double electron_temperature;
	double effective_charge;
	double electric_field;
	double runaway_density; 
};

typedef std::vector<cell> profile;

double runafluid_control(double electron_density, double rundensity_before, double electron_temperature,
		double effective_charge, double electric_field, double timestep, int runafluid_switch, double *rate_values);
		
		
#endif /* CONTROL_H_ */
