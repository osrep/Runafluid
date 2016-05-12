#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../cpo_utils.h"

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

/*
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
		
	
		celll.electric_field = coreprof.profiles1d.eparallel.value(rho); 

		

		pro.push_back(celll);
	}

	return pro;
	
}*/


void fire(ItmNs::Itm::coreprof &coreprof, double &ne_value, int &ne_switch, double &output) {		
		
	try {
		
		bool bools[1];
		
		int swint = int_switch(ne_switch,bools,sizeof(bools)/sizeof(bool));
		
			
		int rho = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double ne_value2;
							
		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			if(bools[0]){
				ne_value2 = (double)rho/(coreprof.ne.value.rows()-1.0)*ne_value;
			}else{
				ne_value2 = ne_value;
			}
			coreprof.ne.value(rho) = ne_value2;
			
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
