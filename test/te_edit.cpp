#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../cpo_utils.h"
#include "../critical_field.cpp"

/*! 

Parallel electric field editor

temp_switch:

AB

 A
   0: relative field
   1: absolute field 

 B  
   0: related to Dreicer field
   1: related to critical field


*/


/*!

read_coreprof

Reading coreprof to a profile. Other fields would be undefined!

input: ItmNs::Itm::coreprof &coreprof
output: profile
*/
profile read_coreprof(const ItmNs::Itm::coreprof &coreprof) {

	profile pro;

	//! read electron density profile length of dataset: cells	
	int cells = coreprof.ne.value.rows();
	
	//! read electron temperature profile length of dataset, comparing with cells
	if (coreprof.te.value.rows() != cells)
		throw std::invalid_argument("Number of values is different in coreprof ne and te.");

	//! read eparallel profile length of dataset, comparing with cells
	if (coreprof.profiles1d.eparallel.value.rows() != cells)
		throw std::invalid_argument(
				"Number of values is different in coreprof.ne and coreprof.profiles1d.eparallel.");

    //! read data in every $\rho$ 

	for (int rho = 0; rho < cells; rho++) {
		cell celll;
				
		//! electron density
		celll.electron_density = coreprof.ne.value(rho);
		
		//! electron temperature
		celll.electron_temperature = coreprof.te.value(rho);
		
		/*! local electric field
			\f[ E = \frac{E_\parallel(\rho) B_0}{B_\mathrm{av}(\rho)} \f]
			where B_\mathrm{av} is known on discreate \f$R \f$ major radius and interpolated at $\rho$ normalised minor radius
		*/
		celll.electric_field = coreprof.profiles1d.eparallel.value(rho) /** coreprof.toroid_field.b0
				/ interpolate(equilibrium.profiles_1d.rho_tor, equilibrium.profiles_1d.b_av,
						coreprof.rho_tor(rho))*/;


		

		pro.push_back(celll);
	}

	return pro;
}


void fire(ItmNs::Itm::coreprof &coreprof, double &Te_value, int &Te_switch, double &output) {		
		
	try {
		
		bool bools[1];
		
		int swint = int_switch(Te_switch,bools,sizeof(bools)/sizeof(bool));		
			
		int rho = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double Te_value2;
		
					
		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {

			if(bools[0]){
				Te_value2 = (double)rho/(coreprof.ne.value.rows()-1.0)*Te_value;
			}else{
				Te_value2 = Te_value;
			}
			coreprof.te.value(rho) = Te_value2;
			
			rho++;
		
		}	
		
		output += (double)swint;	


	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_ILLEGAL_INT;
		
		//! internal error in distribution
		
	}

}
