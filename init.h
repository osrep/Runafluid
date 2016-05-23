#ifndef INIT_H_
#define INIT_H_

#include <vector>

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
		double effective_charge, double electric_field, double timestep, int runafluid_switch);
		
		
#endif /* INIT_H_ */
