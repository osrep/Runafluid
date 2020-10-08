#include <stdexcept>
#include <UALClasses.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "constants.h"
#include "distinit.h"
#include "control.h"


double sign(double a){

	double b;
	if(a>0){
		b=1.0;
	}else if (a==0){
		b=0.0;
	}else{
		b=-1.0;
	}
	
	return b;
}





// Binary search
int binary_search(const Array<double, 1> &array, int first, int last, double search_key) {
	if (first == last)
		return first;

	if (first > last)
		return -1;

	int mid = (first + last) / 2;
	if (array(mid) <= search_key && search_key < array(mid + 1))
		return mid;

	if (search_key < array(mid))
		return binary_search(array, first, mid - 1, search_key);
	else
		return binary_search(array, mid + 1, last, search_key);
}

int binary_search(const Array<double, 1> &array, double search_key) {
	return binary_search(array, 0, array.rows() - 2, search_key);
}



// Linear interpolation
double interpolate(const Array<double, 1> &x, const Array<double, 1> &y, double xa) {

	int rows = x.rows();
	
	
	if (rows == 0){
		std::cerr << "  [Runaway Fluid] ERROR: Number of rows is zero." << std::endl;
		throw std::invalid_argument("Number of rows is zero.");
	}
	
	if (rows != y.rows()){
		std::cerr << "  [Runaway Fluid] ERROR: Number of rows is different in arrays to interpolate." << std::endl;
		throw std::invalid_argument("Number of rows is different in arrays to interpolate.");
	}

	if (xa <= x(0))
		return y(0);
	if (xa >= x(x.rows() - 1))
		return y(y.rows() - 1);

	int index = binary_search(x, xa);
	if (index < 0){
		std::cerr << "  [Runaway Fluid] ERROR: Binary search failed." << std::endl;
		throw std::invalid_argument("Binary search failed.");
	}
	return y(index) + (y(index + 1) - y(index)) / (x(index + 1) - x(index)) * (xa - x(index));
}


/* This code is looking for
which element of distri_vec is 
the runaway distribution

runaway: distri_vec index
no runaway: -1

*/
int whereRunaway(const ItmNs::Itm::distribution &distribution){
	int N_distr = 0;
	
	int runaway_index = -1;
	
	try {
		N_distr = distribution.distri_vec.rows();
		
		for (int i=0; (i<N_distr && runaway_index<0); i++){

			// Is the distribution flag the runaway DISTSOURCE_IDENTIFIER (7)?
			if (distribution.distri_vec(i).source_id.rows()>0){
				if (distribution.distri_vec(i).source_id(0).type.flag == DISTSOURCE_IDENTIFIER){
					runaway_index = i;
				}
			}
		}
		
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR : An error occurred during geometry vectors allocation" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR : " << ex.what() << std::endl;
	}
		
	return runaway_index;

}

/*!

Copy data from CPO inputs to profile structure

*/
		
plasma_profile cpo_to_profile(const ItmNs::Itm::coreprof &coreprof, const ItmNs::Itm::coreimpur &coreimpur,
		const ItmNs::Itm::equilibrium &equilibrium, const ItmNs::Itm::distribution &distribution){		

	plasma_profile pro;
	double number_of_parts;

	// read electron density profile length of dataset: cell_length	
	int plasmaProfileLength = coreprof.ne.value.rows();
	
	// read electron temperature profile length of dataset, comparing with cell_length
	if (coreprof.te.value.rows() != plasmaProfileLength){
		throw std::invalid_argument("  [Runaway Fluid] Number of values is different in coreprof ne and Te.");		
	}		
	
	// read eparallel profile length of dataset, comparing with cell_length
	if (coreprof.profiles1d.eparallel.value.rows() != plasmaProfileLength){		
		throw std::invalid_argument(
				"  [Runaway Fluid] Number of values is different in coreprof.ne and coreprof.profiles1d.eparallel.");		
	}			
													
						
	// read distribution source index for runaways from distribution CPO						
	int distsource_index = whereRunaway(distribution);	
								
    	// read data in every rho 

	for (int i = 0; i < plasmaProfileLength; i++) {
		plasma_local plasmaLocal;
		
		// normalised minor radius
		plasmaLocal.rho = coreprof.rho_tor_norm(i);
				
		// electron density
		plasmaLocal.electron_density = coreprof.ne.value(i);
		
		// electron temperature
		plasmaLocal.electron_temperature = coreprof.te.value(i);
		
		// parallel electric field
		plasmaLocal.electric_field = coreprof.profiles1d.eparallel.value(i);
		
		try{		
			// local magnetic field
			plasmaLocal.magnetic_field = interpolate(equilibrium.profiles_1d.rho_tor, equilibrium.profiles_1d.b_av,
							coreprof.rho_tor(i));
			
		} catch (const std::exception& ex) {
			plasmaLocal.magnetic_field = 0;			
			std::cerr << "  [Runaway Fluid] ERROR : in magnetic field, magnetic field set to zero. (" << i << ")" << std::endl;
		}
		
		try{		
			// No runaway in previous distribution CPO
			if (distsource_index<0){
				plasmaLocal.runaway_density = 0;
			// Runaway in previous distribution CPO
			}else{
				plasmaLocal.runaway_density = distribution.distri_vec(distsource_index).profiles_1d.state.dens(i);
			}

		// internal error in distribution
		} catch (const std::exception& ex) {

			plasmaLocal.runaway_density = 0;
			
			std::cerr << "  [Runaway Fluid] WARNING : Cannot read runaway density, density set to zero." << std::endl;
		}

		// total sum of electric charge from coreprof CPO
		plasmaLocal.effective_charge = coreprof.profiles1d.zeff.value(i);	

		pro.push_back(plasmaLocal);
	}

	return pro;
}		
