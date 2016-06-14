#include <stdexcept>
#include <iostream>
#include "control.h"
//#include "products.h"
//#include "critical_field.h"
#include "critical_field.cpp"
//#include "dreicer.h"
#include "dreicer.cpp"
//#include "avalanche.h"
#include "avalanche.cpp"

//#include "cpo_utils.cpp"
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
		std::cerr << "DREICER RATE: "  << rate_dreicer << std::endl;		
		
		//! Calculate Avalanche generation rate
		rate_avalanche = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, 0);
		rate_values[1] = rate_avalanche;
		std::cerr << "AVALANCHE RATE: "  << rate_avalanche << std::endl;			
		
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
		std::cerr << "ERROR An error occurred during runaway distribution calculation." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		rundensity_after = ITM_ILLEGAL_FLOAT;
		
	}
	return rundensity_after;
}
