#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../cpo_utils.h"
#include "../constants.h"

/*! 

Explicit electron energy source term editor: sets coresource/values/qe/exp 
to a given value for the whole profile.

qe_imp: implicit energy source [s^-1.m^-3]

*/

void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coresource &coresource, ItmNs::Itm::equilibrium &equilibrium, double &exponential_time_constant, double &temperature_limit) {		
		
	try {
		
		std::cerr << " START: runafluid_qe_exp_edit" << std::endl;	
			
		int i = 0;
		double volume = 0;
		double volume_prev = 0;
		int values_index = 0;
		int source_id = 0;

		//! reading profile from CPO inputs
		profile pro = cpo_to_profile(coreprof);
		coresource.values(values_index).sourceid.flag = source_id;
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it,++i) {				
			coresource.values(values_index).qe.exp(i) = (it->electron_temperature-temperature_limit)*ITM_EV * it->electron_density / exponential_time_constant;							
			coresource.values(values_index).qi.exp(i) = coreprof.ti.value(i)*ITM_EV * coreprof.ni.value(i) / exponential_time_constant;	
		}	
		
		std::cerr << " END: runafluid_qe_exp_edit" << std::endl;

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_qe_exp_edit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
	}

}
