#include <stdexcept>
#include <iostream>
#include "control.h"
#include "critical_field.h"
#include "dreicer.h"
#include "avalanche.h"
#include "cpo_utils.h"

/*!
Runafluid actor

timestep: in s

*/

double runafluid_control(double electron_density, double rundensity_before, double electron_temperature,
		double effective_charge, double electric_field, double timestep, int runafluid_switch, double *rate_values){
	
	double rundensity_after = 0.0;
	double rate_dreicer = 0.0;
	double rate_avalanche = 0.0;	
	int dreicer_formula_id = 63;	
		
	int runafluid_booln = 4;
	bool runafluid_bools[runafluid_booln];
	int_switch(runafluid_switch,runafluid_bools,runafluid_booln);
		
	try {	
		
		//! Dreicer rate formula		 
		if (runafluid_bools[1]){
			dreicer_formula_id = 63;
		}else{				
			if (runafluid_bools[2]){				
				 dreicer_formula_id = 66;
			}else{
				 dreicer_formula_id = 67;
			}
		}
		
		
		//! Calculate Dreicer generation rate
		rate_dreicer = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, dreicer_formula_id);
		rate_values[0] = rate_dreicer*electron_density;
		
		//! temporary for 63,66,67		
		rate_values[2] = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, 63)*electron_density;
		rate_values[3] = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, 66)*electron_density;
		rate_values[4] = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, 67)*electron_density;
		rate_values[5] = rundensity_before;
		
		//! temporary for Dreicer field
		rate_values[6] = calculate_dreicer_field(electron_density, electron_temperature);
		rate_values[7] = calculate_dreicer_field2(electron_density, electron_temperature);
		rate_values[8] = me_c2 / electron_temperature / ITM_EV / calculate_critical_field(electron_density, electron_temperature);
		
		//! temporary for critical field
		//rate_values[8] = calculate_critical_field(electron_density, electron_temperature);
		
		//! temporary for Coulomb log
		rate_values[9] = calculate_coulomb_log(electron_density, electron_temperature);
		
		//! Calculate Avalanche generation rate
		rate_avalanche = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, 0);
		rate_values[1] = rate_avalanche;		
		
		// Dreicer on
		if (runafluid_bools[0]==false){
			rate_dreicer = 0;		
		}
		
		// avalanche on
		if (runafluid_bools[3]==false){
			rate_avalanche = 0;		
		}	
			
		/*! runaway electron density			
		n_R = n_R0 + (R_DR+R_A)*dt
		
		\f[
			n_\mathrm{R} (t) = n_\mathrm{R} (t-\Delta t) + \left( n_\mathrm{e} (t) * \gamma_\mathrm{D} (t) + n_\mathrm{R} (t-\Delta t) * \gamma_\mathrm{A} (t) \right) \cdot \Delta t
		
		\f]
		*/	
		
		rundensity_after = rundensity_before + (electron_density*rate_dreicer + rundensity_before*rate_avalanche) * timestep;		


	} catch (const std::exception& ex) {
		//! internal error in runaway distribution calculation
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during runaway distribution calculation." << std::endl;
		std::cerr << "[Runaway Fluid] ERROR : " << ex.what() << std::endl;
		rundensity_after = ITM_ILLEGAL_FLOAT;
		
	}
	return rundensity_after;
}
