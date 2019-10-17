#ifndef CONTROL_H_
#define CONTROL_H_
	
#include <vector>
#include <string>

// cell structure	
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

struct module_struct {
	std::string dreicer_formula;
	bool dreicer_toroidicity;
	std::string avalanche_formula;
	bool avalanche_toroidicity;
	std::string output_path;
	double warning_fraction_limit;
	double rho_edge_calculation_limit;
	
};

double runafluid_control(double electron_density, double rundensity_before, double electron_temperature,
		double effective_charge, double electric_field, double magnetic_field, double timestep, double inv_asp_ratio, double rho_tor_norm, module_struct modules, double *rate_values);

int runafluid_switch_message(module_struct modules);
		
#endif /* CONTROL_H_ */
