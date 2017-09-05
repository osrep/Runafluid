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

void fire(ItmNs::Itm::coreprof &coreprof, double &te_value, int &te_switch, double &output) {		
		
	try {
		
		//! start: runafluidteEdit
		std::cerr << " START: runafluid_teEdit" << std::endl;
		bool bools[2];
		
		int swint = bool_switch(te_switch,bools,sizeof(bools)/sizeof(bool));		
			
		int i = 0;
		double critical_field = 0;
		double dreicer_field = 0;

		double te_value2;

		//! reading profile from CPO inputs
		profile pro = cpo_to_profile(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {	

			if(bools[1]){			
				if(bools[0]){
					te_value2 = pow(10,(double)i/(coreprof.te.value.rows()-1.0)*log10(te_value));
				}else{
					te_value2 = (double)i/(coreprof.te.value.rows()-1.0)*te_value;
				}					
			}else{
				te_value2 = te_value;
			}
			
			coreprof.te.value(i) = te_value2;
			
			i++;
		
		}	
		
		output = 0;	

		//! end: runafluid_teEdit
		std::cerr << " END: runafluid_teEdit" << std::endl;

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_teEdit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_INVALID_INT;		
		
	}

}
