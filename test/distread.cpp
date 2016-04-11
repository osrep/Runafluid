#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../cpo_utils.h"
#include "../runafluid.h"
#include "../init.h"



profile read_distribution(const ItmNs::Itm::distribution &distribution) {

	profile pro;

	//! read electron density profile length of dataset: cells	
	int cells = distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.rows();
	

    //! read data in every $\rho$ 

	for (int rho = 0; rho < cells; rho++) {
		cell celll;
				
		//! runaway distribution

		celll.runaway_density = distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho);
		

		pro.push_back(celll);
	}

	return pro;
}


/*! 

runaway distribution reader

*/
void fire(ItmNs::Itm::distribution &distribution, double &rundensity) {
		
		
		try {
			
		//double 
		
		int rho = 0;
		int index_back=0;
		rundensity = 0.0;
		
		//! reading profile from CPO inputs
		profile pro = read_distribution(distribution);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			//rundensity = 1234.5678;//
			if(rho<=index_back){
				rundensity = it->runaway_density;
			}
			rho++;
		
		
		}		
		
	rundensity = 4224.4224;			
	

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
