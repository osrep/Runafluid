#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../cpo_utils.h"

/*! 

Parallel electric field editor

te_switch:

AB

 A
   0: non-increasing temperature
   1: increasing temperature   
 
 B  
   0: linear
   1: logarithmic


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


void fire(ItmNs::Itm::coreprof &coreprof, double &te_value, int &te_switch, double &output) {		
		
	try {
		
		bool bools[2];
		
		int swint = int_switch(te_switch,bools,sizeof(bools)/sizeof(bool));		
			
		int rho = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double te_value2;
		
					
		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {	

			if(bools[1]){			
				if(bools[0]){
					te_value2 = pow(10,(double)rho/(coreprof.ne.value.rows()-1.0)*log10(te_value));
				}else{
					te_value2 = (double)rho/(coreprof.ne.value.rows()-1.0)*te_value;
				}					
			}else{
				te_value2 = te_value;
			}
			
			coreprof.te.value(rho) = te_value2;
			
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
