#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "constants.h"
#include "ids_utils.h"
#include "critical_field.h"

/*! 

Parallel electric field editor

electric_field_switch:

ABCD (Do not write strating 0s!)

 A
   0: non-increasing field 
   1: increasing field   
 
 B  
   0: linear
   1: logarithmic

 C
   0: absolute field
   1: relative field 

 D  
   0: related to Dreicer field
   1: related to critical field

*/


void fire(IdsNs::IDS::core_profiles &core_profiles,  IdsNs::IDS::equilibrium &equilibrium, double &electric_field_value, int &electric_field_switch, double &output) {
		
	try {
		
		//! start: runafluid_efieldEdit
		std::cerr << " START: runafluid_efieldEdit" << std::endl;

		//! bool array initialisation	
		bool bools[4];
		
		//! set switches
		int swint = bool_switch(electric_field_switch,bools,sizeof(bools)/sizeof(bool));
		
		//! counter initialisation			
		int i = 0;
		int timeindex = 0;
		double critical_field = 0;
		double dreicer_field = 0;
		
		double electric_field_value2;
					
		//! reading profile from CPO inputs
		profile pro = ids_to_profile(core_profiles);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			if(bools[3]){
			
				//! logarithmically increasing field
				if(bools[2]){
					electric_field_value2 = pow(10,(double)i/(core_profiles.profiles_1d(timeindex).electrons.density.rows()-1.0)*log10(electric_field_value));
					
				//! linearly increasing field
				}else{
					electric_field_value2 = (double)i/(core_profiles.profiles_1d(timeindex).electrons.density.rows()-1.0)*electric_field_value;
				}	
			//! constant field
			}else{
				electric_field_value2 = electric_field_value;
			}
			
			if(bools[1]){				
				if(bools[0]){
				
					//! maximal value of field related to critical field
					critical_field = calculate_critical_field(it->electron_density, it->electron_temperature);				
					critical_field/= ( equilibrium.vacuum_toroidal_field.b0(timeindex) / 					
					interpolate(equilibrium.time_slice(timeindex).profiles_1d.rho_tor, equilibrium.time_slice(timeindex).profiles_1d.b_field_average,
						core_profiles.profiles_1d(timeindex).grid.rho_tor(i)) );

					core_profiles.profiles_1d(timeindex).e_field.parallel(i) = electric_field_value2*critical_field;		
					
				} else {
				
					//! maximal value of field related to Dreier field
					dreicer_field = calculate_dreicer_field(it->electron_density, it->electron_temperature);
					core_profiles.profiles_1d(timeindex).e_field.parallel(i) = electric_field_value2*dreicer_field;	
				}
				
			} else {
			
			//! absolut electric field
				core_profiles.profiles_1d(timeindex).e_field.parallel(i) = electric_field_value2;
			}
			
			i++;
		
		}				
		
		output = 0;
		
		//! end: runafluid_efieldEdit
		std::cerr << " END: runafluid_efieldEdit" << std::endl;
	

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_efieldEdit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_INVALID_INT;		
		
	}

}
