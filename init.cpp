#include <stdexcept>
#include <iostream>
#include "init.h"
//#include "products.h"
//#include "critical_field.h"
#include "critical_field.cpp"
//#include "dreicer.h"
#include "dreicer.cpp"
//#include "avalanche.h"
#include "avalanche.cpp"

/*!
Runafluid actor

*/

double runafluid_control(double electron_density, double rundensity, double electron_temperature,
		double effective_charge, double electric_field, double timestep){
	
	double rundensity_before = 0.0;
	double rundensity_after = 0.0;
	double rate_dreicer = 0.0;
	double rate_avalanche = 0.0;
		
	try {
	
		//! Calculate Dreicer generation rate
		rate_dreicer = dreicer_generation_rate(electron_density, electron_temperature,
		effective_charge, electric_field);
		
		//! Calculate Dreicer generation rate
		rate_avalanche = avalanche_generation_rate(rundensity_before, electron_temperature,
		effective_charge, electric_field, 0);
		
		/* runaway electron density	
		
		n_R = (R_DR+R_A)*dt
		*/
		
		rundensity_after = (rate_dreicer + rate_avalanche) * timestep;					

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}
	return rundensity_after;
}
