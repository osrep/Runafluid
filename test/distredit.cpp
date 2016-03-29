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
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution) {
		
		
		try {
			
		double rundensity = 0.0;
		double rundensity2 = 1234.56;
		
		//! reading profile from CPO inputs
		profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			//rundensity = 1234.5678;//
			rundensity = it->runaway_density;
			distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = rundensity2;
		
		
		}		
		
				
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}

}
