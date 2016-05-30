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

double runafluid_control(double electron_density, double rundensity_before, double electron_temperature,
		double effective_charge, double electric_field, double timestep, int runafluid_switch){
	
	//double rundensity_before = 0.0;
	double rundensity_after = 0.0;
	double rate_dreicer = 0.0;
	double rate_avalanche = 0.0;
		
	try {
	
		//! Calculate Dreicer generation rate
		rate_dreicer = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field);
		
		//! Calculate Dreicer generation rate
		rate_avalanche = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, 0);
		
		/* runaway electron density	
		
		n_R = (R_DR+R_A)*dt
		*/
		
		int runafluid_booln=3;
		bool runafluid_bool[runafluid_booln];
		rundensity_after = rundensity_before + (electron_density*rate_dreicer + rundensity_before*rate_avalanche) * timestep;		
		
		
		int_switch(runafluid_switch,runafluid_bools,runafluid_booln);
		if (runafluid_bool[2]){
			rundensity_after = rundensity_before;			
			std::cerr << "RUNDENSITY BEFORE: "  << std::endl;	
		}
		
		if (runafluid_bool[1]){
			rundensity_after = rate_dreicer ;	
			std::cerr << "DREICER: "  << std::endl;		
		}
		if (runafluid_bool[0]){
			rundensity_after = rate_avalanche;	
			std::cerr << "AVALANCHE: "  << std::endl;		
		}
	
	
		

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during runing runafluid_control." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		rundensity_after = ITM_ILLEGAL_INT;
		//! internal error in distribution
		
	}
	return rundensity_after;
}
