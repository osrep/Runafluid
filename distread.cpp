#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "constants.h"
#include "cpo_utils.h"
#include "runafluid.h"
#include "init.h"


/*! 

runaway distribution reader

*/
void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution, double &rundensity) {
		
		
		try {
			
		//double rundensity = 0.0;
		
		//! reading profile from CPO inputs
		profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			//rundensity = 1234.5678;//
			rundensity = it->runaway_density;
		
		
		}		
		
				
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}

	/*try {
			
		//! internal error in distribution
		rundensity=8136.5;
		
		//! reading profile from CPO inputs
		profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			rundensity=124.5;//it->runaway_density;
		
		
		}		
		
				
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		rundensity=ITM_ILLEGAL_INT;
		
	}*/
}


//distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(0);
