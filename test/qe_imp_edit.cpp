#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "../cpo_utils.h"

/*! 

Implicit electron energy source term editor: sets coresource/values/qe/imp 
to a given value for the whole profile.

qe_imp: Implicit energy source [s^-1.m^-3]

*/

void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coresource &coresource, double &qe_imp, int &values_index, int &source_id, double &output) {		
		
	try {
		
		std::cerr << " START: runafluid_qe_imp_edit" << std::endl;	
			
		int rho = 0;

		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		coresource.values(values_index).sourceid.flag = source_id;
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {	
		
			//coresource.values(values_index).qe.imp(rho) = qe_imp;   //HACK!! back!
			coresource.values(values_index).qe.exp(rho) = qe_imp/0.01;//HACK!!! remove
			
			rho++;
		
		}	
		
		output = 0;	

		std::cerr << " END: runafluid_qe_imp_edit" << std::endl;

	} catch (const std::exception& ex) {
	
		//! internal error in distribution
		std::cerr << "ERROR An error occurred during firing actor runafluid_qe_imp_edit." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_INVALID_INT;		
		
	}

}
