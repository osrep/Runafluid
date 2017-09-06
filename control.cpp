#include <stdexcept>
#include <iostream>
#include "control.h"
#include "critical_field.h"
#include "dreicer.h"
#include "avalanche.h"
#ifdef ITM_CONSTANTS
    #include "cpo_utils.h"
#else
    #include "ids_utils.h"
#endif
#include "products.h"

/*!
Runafluid actor

timestep: in s

*/

double runafluid_control(double electron_density, double rundensity_before, double electron_temperature,
		double effective_charge, double electric_field, double magnetic_field, double timestep, double inv_asp_ratio, double rho_tor_norm, int runafluid_switch, double *rate_values){
	
	double rundensity_after = 0.0;
	double rate_dreicer = 0.0;
	double rate_avalanche = 0.0;	
	int dreicer_formula_id = 63;	
		
	try {	
				
		//! get module variables
		int	modulevar_rates = get_digit(runafluid_switch,1);
		int modulevar_dreicer = get_digit(runafluid_switch,2);
		int	modulevar_avalanche = get_digit(runafluid_switch,3);	
		int	modulevar_toroidicity = get_digit(runafluid_switch,4);	
		
		//! choose Dreicer module scenario
		if (modulevar_dreicer==1) {dreicer_formula_id = 63;}
		if (modulevar_dreicer==2) {dreicer_formula_id = 66;}
		if (modulevar_dreicer==3) {dreicer_formula_id = 67;}
				
		//! Calculate Dreicer generation rate
		rate_dreicer = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, rho_tor_norm, dreicer_formula_id);			
		
		//! Calculate Avalanche generation rate
		rate_avalanche = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, magnetic_field, modulevar_avalanche);
		// Dreicer on
		if (modulevar_dreicer==0){
			rate_dreicer = 0;		
		}		

		// avalanche on
		if (modulevar_avalanche==0){
			rate_avalanche = 0;		
		}		
		
		/*! toroidicity for Dreicer rate	
		
		
		*/		
		if (modulevar_toroidicity == 1 || modulevar_toroidicity == 2){
			rate_dreicer *= calculate_toroidicity_dreicer(inv_asp_ratio, rho_tor_norm);
		}
		
		/*! toroidicity for Avalanche rate	
		
		*/		
		if (modulevar_toroidicity == 1 || modulevar_toroidicity == 3){
			rate_avalanche *= calculate_toroidicity_avalanche(inv_asp_ratio, electric_field, electron_density, electron_temperature, rho_tor_norm);
		}	
		// temporary data back module
		if (modulevar_rates!=0){	
				
			//! temporary for Dreicer rate
			rate_values[0] = rate_dreicer*electron_density;
		
			//! temporary for Avalanche rate
			rate_values[1] = rate_avalanche;
			if(modulevar_rates == 1){		
				//! temporary for Dreicer H&C 63,66,67		
				rate_values[2] = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, rho_tor_norm, 63)*electron_density;
				rate_values[3] = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, rho_tor_norm, 66)*electron_density;
				rate_values[4] = dreicer_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, rho_tor_norm, 67)*electron_density;
				rate_values[5] = rundensity_before;
		
				//! temporary for Dreicer field
				rate_values[6] = calculate_dreicer_field(electron_density, electron_temperature);
		
				//! temporary for critical field
				double critical_field = calculate_critical_field(electron_density, electron_temperature);
				rate_values[7] = critical_field;
		
				//! temporary for Coulomb log
				rate_values[8] = calculate_coulomb_log(electron_density, electron_temperature);		
		
		
				//! temporary for collision time
				rate_values[9] = calculate_thermal_electron_collision_time(electron_density, electron_temperature);
				rate_values[10] = calculate_runaway_collision_time(electron_density, electron_temperature);		
			
				//! temporary for Dreicer H&C 63,66,67		
				rate_values[11] = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, magnetic_field, 1);
				rate_values[12] = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, magnetic_field, 2);
				rate_values[13] = avalanche_generation_rate(electron_density, electron_temperature, effective_charge, electric_field, magnetic_field, 3);

				//! temporary time data
				double runaway_collision_time = calculate_runaway_collision_time(electron_density, electron_temperature);	
				double synchrotron_loss_time = calculate_synchrotron_loss_time(magnetic_field);
				double norm_synchrotron_loss_time = synchrotron_loss_time/runaway_collision_time;
		
				rate_values[14] = 1 + (1+effective_charge) / sqrt(norm_synchrotron_loss_time) / pow( 1.0/8.0 + (1+effective_charge) * (1+effective_charge) / norm_synchrotron_loss_time , 1.0/6.0);
		
				rate_values[15] = synchrotron_loss_time;
				rate_values[16] = norm_synchrotron_loss_time;
				
				//! temporary toroidicity data
				rate_values[17] = calculate_toroidicity_dreicer(inv_asp_ratio, rho_tor_norm);				
				rate_values[18] = calculate_toroidicity_avalanche(inv_asp_ratio, electric_field, electron_density, electron_temperature, rho_tor_norm);
				
				//! temporary for relative electric field				
				rate_values[19] =  electric_field/critical_field;
			}
		}

			
		/*! runaway electron density			
		n_R = n_R0 + (R_DR+R_A)*dt
		
		\f[
			n_\mathrm{R} (t) = n_\mathrm{R} (t-\Delta t) + \left( n_\mathrm{e} (t) * \gamma_\mathrm{D} (t) + n_\mathrm{R} (t-\Delta t) * \gamma_\mathrm{A} (t) \right) \cdot \Delta t
		
		\f]
		*/	
		
		//! runaway dansity after (return)
		rundensity_after = rundensity_before + (electron_density*rate_dreicer + rundensity_before*rate_avalanche) * timestep;		


	} catch (const std::exception& ex) {
		//! internal error in runaway distribution calculation
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during runaway distribution calculation." << std::endl;
		std::cerr << "[Runaway Fluid] ERROR : " << ex.what() << std::endl;
		rundensity_after = ITM_INVALID_FLOAT;
		
	}
	return rundensity_after;
}


/*! Runafluid switch message */

int runafluid_switch_message(int runafluid_switch){
	
			
	int	modulevar_rates = get_digit(runafluid_switch,1);
	int modulevar_dreicer = get_digit(runafluid_switch,2);
	int	modulevar_avalanche = get_digit(runafluid_switch,3);	
	int	modulevar_toroidicity = get_digit(runafluid_switch,4);	
	int	modulevar_5 = get_digit(runafluid_switch,5);
	int dreicer_formula_id = 63;
		
			
	std::cerr << "  [Runaway Fluid] Warning: A new Runaway_Fluid actor released where runafluid_switch changed. Please read documentation about how to use runafluid_switch!"<< std::endl;	
	std::cerr << "\t\t\tMore info:\thttp://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runafluid-usermanual"<< std::endl;	
	
	
	
	//! choose Dreicer module scenario
	if (modulevar_dreicer==1) {dreicer_formula_id = 63;}
	if (modulevar_dreicer==2) {dreicer_formula_id = 66;}
	if (modulevar_dreicer==3) {dreicer_formula_id = 67;}	
	
	//! Dreicer message
	if (modulevar_dreicer == 0){		
		std::cerr << "  [Runaway Fluid] \tDreicer module OFF"<< std::endl;	
	}else{
		std::cerr << "  [Runaway Fluid] \tDreicer module ON"<< std::endl;	
		std::cerr << "\t\t\twith H&C (" << dreicer_formula_id << ") formula"<< std::endl;	
		if (dreicer_formula_id != 63){
			std::cerr << "\t\t\tPlease use formula (63)!"<< std::endl;
			std::cerr << "\t\t\tMore info:\thttp://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runafluid-usermanual"<< std::endl;	
		}
	}
		
	//! avalanche message
	if (modulevar_avalanche == 0){		
		std::cerr << "  [Runaway Fluid] \tAvalanche OFF"<< std::endl;	
	}else{
		std::cerr << "  [Runaway Fluid] \tAvalanche module ON (modulevar: " << modulevar_avalanche << ")" << std::endl;		
	}
	
	
	//! toroidicity message
	if (modulevar_toroidicity == 0){		
		std::cerr << "  [Runaway Fluid] \tToroidicity module OFF"<< std::endl;	
		
	}else if (modulevar_toroidicity == 1) {
		std::cerr << "  [Runaway Fluid] \tToroidicity module ON"<< std::endl;	
		
	}else if (modulevar_toroidicity == 2) {
		std::cerr << "  [Runaway Fluid] \tToroidicity module ONLY for Dreicer"<< std::endl;	
		
	}else if (modulevar_toroidicity == 3) {
		std::cerr << "  [Runaway Fluid] \tToroidicity module ONLY for Avalanche"<< std::endl;	
	}
	
}
