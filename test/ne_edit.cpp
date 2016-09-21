#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../constants.h"
#include "../cpo_utils.h"

/*! 

Parallel electric field editor

ne_switch:

AB

 A
   0: non-increasing density
   1: increasing density   
 
 B  
   0: linear
   1: logarithmic


*/


void fire(ItmNs::Itm::coreprof &coreprof, double &ne_value, int &ne_switch, double &output) {		
		
	try {
		
		
		//! start: runafluid_neEdit
		std::cerr << " START: runafluid_neEdit" << std::endl;
		
		bool bools[2];
		
		int swint = int_switch(ne_switch,bools,sizeof(bools)/sizeof(bool));
		
			
		int rho = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double ne_value2;
							
		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			if(bools[1]){			
				if(bools[0]){
					ne_value2 = pow(10,(double)rho/(coreprof.ne.value.rows()-1.0)*log10(ne_value));
				}else{
					ne_value2 = (double)rho/(coreprof.ne.value.rows()-1.0)*ne_value;
				}					
			}else{
				ne_value2 = ne_value;
			}
			
			coreprof.ne.value(rho) = ne_value2;
			
			rho++;
		
		}	
		
		output = 0;		
		
		
		//! end: runafluid_neEdit
		std::cerr << " END: runafluid_neEdit" << std::endl;

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_neEdit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_ILLEGAL_INT;		
		
	}

}
