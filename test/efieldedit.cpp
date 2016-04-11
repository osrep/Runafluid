#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../init.h"
#include "../cpo_utils.h"
#include "../critical_field.cpp"

/*! 

runaway distribution editor

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

/*
void int_switch(int input_switch, bool &sw1, bool &sw2){

	if (input_switch==1){
		&sw1=true;
	}

}*/

void fire(ItmNs::Itm::coreprof &coreprof, double &electric_field, int &input_switch, double &output) {
		
		
	try {


		bool $relefield = false;
		bool $dreicer = false;
		
		/*! SWITCH
		relative / absolute
		dreicer / critical
		
		*/
		
//		int_switch(input_switch,&$relefield,&$dreicer);
		
		if (input_switch % 2 == 0){
			$relefield = true;			
			output = 9.87654321;
		}else{				
			output = 1.23456789;
		}
		
			
		int rho = 0;
		double critical_field = 0;
		double dreicer_field = 0;
//		output = 0;
		

		
		
					
		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			if ($relefield){
				output = 98765.4321;
				critical_field = calculate_critical_field(it->electron_density, it->electron_temperature);
				coreprof.profiles1d.eparallel.value(rho) = electric_field/critical_field;
			} else {
			
			//! absolut electric field
				output = 12345.6789;//coreprof.profiles1d.eparallel.value(rho);
				coreprof.profiles1d.eparallel.value(rho) = electric_field;
			}
			
			rho++;
		
		}		
		output += (rho*10);
				
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_ILLEGAL_INT;
		
		//! internal error in distribution
		
	}

}
