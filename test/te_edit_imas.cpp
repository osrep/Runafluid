#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../ids_utils.h"

/*! 

Parallel electric field editor

te_switch:

AB

 A
   0: non-increasing temperature
   1: increasing temperature   
 
 B  
   0: linear
   1: logarithmic

*/

void fire(IdsNs::IDS::core_profiles &core_profiles, double &te_value, int &te_switch, double &output) {		
		
	try {
		
		//! start: runafluidteEdit
		std::cerr << " START: runafluid_teEdit" << std::endl;
		bool bools[2];
		
		int swint = bool_switch(te_switch,bools,sizeof(bools)/sizeof(bool));		
			
		int i = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double te_value2;

		//! reading profile from CPO inputs
		profile pro = ids_to_profile(core_profiles);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {	

			if(bools[1]){			
				if(bools[0]){
					te_value2 = pow(10,(double)i/(core_profiles.profiles_1d(timeindex).electrons.temperature.rows()-1.0)*log10(te_value));
				}else{
					te_value2 = (double)i/(core_profiles.profiles_1d(timeindex).electrons.temperature.rows()-1.0)*te_value;
				}					
			}else{
				te_value2 = te_value;
			}
			
			core_profiles.profiles_1d(timeindex).electrons.temperature(i) = te_value2;
			
			i++;
		
		}	
		
		output = 0;	

		//! end: runafluid_teEdit
		std::cerr << " END: runafluid_teEdit" << std::endl;

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_teEdit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_INVALID_INT;		
		
	}

}
