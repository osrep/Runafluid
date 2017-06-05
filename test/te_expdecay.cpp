#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../cpo_utils.h"

void fire(ItmNs::Itm::coreprof &coreprof, double &time_step, double &exponential_time_constant, double &temperature_limit) {		
		
		
	try {
		
		//! start: runafluidteEdit
		std::cerr << " START: runafluid_te_expdecay" << std::endl;
		
		
		int i = 0;
		double volume = 0;
		double volume_prev = 0;
		int values_index = 0;
		int source_id = 0;

		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it,++i) {				
			coreprof.te.value(i) *= exp(-time_step/exponential_time_constant);	
		}		

		//! end: runafluid_teEdit
		std::cerr << " END: runafluid_te_expdecay" << std::endl;

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_te_expdecay." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
	}

}
