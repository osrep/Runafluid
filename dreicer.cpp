#include <cmath>

#include <stdexcept>
#include <iostream>

#include "dreicer.h"
#include "products.h"
#include "critical_field.h"

using namespace std;

double dreicer_generation_rate(double electron_density, double electron_temperature,
		double effective_charge, double electric_field, double rho_tor_norm, module_struct modules) {
		
	double kB_T = electron_temperature* ITM_EV;
	double coulomb_log = calculate_coulomb_log(electron_density, electron_temperature);
	double thermal_electron_collision_time = calculate_thermal_electron_collision_time(electron_density, electron_temperature);
	double Ed = calculate_dreicer_field(electron_density, electron_temperature);
	double Ed__E = Ed/electric_field;
	double alpha = calculate_alpha(electric_field,electron_density,electron_temperature);
	double lambda = calculate_lambda(alpha);
	double gamma = calculate_gamma(effective_charge,alpha);
	double h = calculate_h(alpha,effective_charge);

	double Cr = 1.0;
	double dgr;
	
	if ( (modules.dreicer_formula == "hc_formula_67") || (modules.dreicer_formula == "hc_formula_66") ){
			dgr = Cr/thermal_electron_collision_time * pow(Ed__E,3.0/16.0*(effective_charge+1.0)) * exp(-1.0/4.0*Ed__E - sqrt((effective_charge+1.0)*Ed__E));
	
		if (modules.dreicer_formula == "hc_formula_66"){
			dgr = dgr * exp(-kB_T/me_c2 * (Ed__E*Ed__E/8.0 + 2.0/3.0*pow(Ed__E,1.5) *sqrt(1.0+effective_charge)));
		}
	
	} else dgr = Cr/thermal_electron_collision_time * pow(Ed__E,h) * exp(-lambda/4.0*Ed__E - sqrt(2.0*Ed__E)*gamma);
	
	// Dreicer rate must be non-negative
	if ( isnan(dgr) || (dgr<0) ) dgr = 0;

	// output: Dreicer generation rate	
	return dgr;
}


// Inner calculations of dreicer generation rate

double calculate_alpha(double electric_field, double electron_density, double electron_temperature){
	return electric_field / calculate_critical_field(electron_density, electron_temperature);
}

double calculate_lambda(double alpha){
	return 8.0*alpha*(alpha-1.0/2.0-sqrt(alpha*(alpha-1.0)));
}

double calculate_gamma(double effective_charge, double alpha){
	return sqrt((1.0+effective_charge) * alpha*alpha/8.0/(alpha-1.0)) * (ITM_PI/2.0-asin(1.0-2.0/alpha));
}
double calculate_h(double alpha, double effective_charge){
	return (1.0/(16.0*(alpha-1.0)) * (alpha*(effective_charge+1.0) -
			effective_charge + 7.0 + 2.0*sqrt(alpha/(alpha-1.0)) * (1.0+effective_charge)*(alpha-2.0)));
}

double calculate_toroidicity_dreicer(double inv_asp_ratio, double rho_tor_norm){
    double inv_asp_ratio_coord = inv_asp_ratio*rho_tor_norm;
	return (1.0 - 1.2*sqrt((2.0*inv_asp_ratio_coord)/(1.0+inv_asp_ratio_coord)));
}

