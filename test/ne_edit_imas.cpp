#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../ids_utils.h"

// Parallel electric field editor

void fire(IdsNs::IDS::core_profiles &core_profiles, double &ne_value, int &ne_switch, double &output) {		
		
	try {

		// start: runafluid_neEdit
		std::cerr << " START: runafluid_neEdit" << std::endl;
		
		bool bools[2];
		
		int swint = bool_switch(ne_switch,bools,sizeof(bools)/sizeof(bool));

		int i = 0;
		int timeindex = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double ne_value2;
							
		// reading profile from CPO inputs
		profile pro = ids_to_profile(core_profiles, timeindex);
		
		// stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			if(bools[1]){			
				if(bools[0]){
					ne_value2 = pow(10,(double)i/(core_profiles.profiles_1d(timeindex).electrons.density.rows()-1.0)*log10(ne_value));
				}else{
					ne_value2 = (double)i/(core_profiles.profiles_1d(timeindex).electrons.density.rows()-1.0)*ne_value;
				}					
			}else{
				ne_value2 = ne_value;
			}
			
			core_profiles.profiles_1d(timeindex).electrons.density(i) = ne_value2;
			
			i++;
		
		}	
		
		output = 0;

		// end: runafluid_neEdit
		std::cerr << " END: runafluid_neEdit" << std::endl;

	} catch (const std::exception& ex) {
	
		// internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_neEdit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_INVALID_INT;		
		
	}

}
