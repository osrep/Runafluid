#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../init.h"
#include "../cpo_utils.h"


/*! 

runaway distribution editor

*/

void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution, double &temp) {
		
		
		try {
			
		int rho = 0;
			
		temp = 0;
		
		//! reading profile from CPO inputs
		profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			coreprof.profiles1d.eparallel.value(rho)*=1e6;
			
			rho++;
		
		}		
		
				
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}

}
