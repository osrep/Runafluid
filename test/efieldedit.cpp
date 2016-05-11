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

electric_field_switch:

ABC

 A
   0: non-increasing field 
   1: increasing field

 B
   0: relative field
   1: absolute field 

 C  
   0: related to Dreicer field
   1: related to critical field


*/

//! N: number of elements in bools
int int_switch(int electric_field_switch, bool *bools, int N){
	
	
	for (int i=0; i<N-1; i++){
		if (electric_field_switch%10==1){
			bools[i]=true;
		}else{		
			bools[i]=false;
		}
		
		
		std::cerr << "BOOL " <<i<< "/" << N-1 << "set as: " << bools[i] << " > switch value " << electric_field_switch << std::endl;
		
		electric_field_switch /= 10;
	}
	
	return N;

}


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


void fire(ItmNs::Itm::coreprof &coreprof, double &electric_field_test, int &electric_field_switch, double &output) {
		
		
	try {


		
		
		/*! SWITCH
		relative / absolute
		dreicer / critical
		
		*/
		
		bool bools[3];// = {$relefield, $dreicer};
		
		std::cerr << "BOOLS size : " << sizeof(bools) << std::endl << std::endl;
		std::cerr << "BOOLS size : " << sizeof(bools)/sizeof(bool) << std::endl << std::endl;
		
		
		int swint = int_switch(electric_field_switch,bools,sizeof(bools)/sizeof(bool));
				
		/*bool $relefield = false;
		bool $dreicer = false;*/
		/*if (electric_field_switch % 10 == 0){
			$relefield = true;			
			output = 9.87654321;
		}else{				
			output = 1.23456789;
		}*/
		
		
		
			
		int rho = 0;
		double critical_field = 0;
		double dreicer_field = 0;
//		double loop_multiplier = 1;
//		output = 0;
		
		double electric_field_test_value;
		
		
		
		
					
		//! reading profile from CPO inputs
		profile pro = read_coreprof(coreprof);
		
		//! stepping iterator in profile		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
			if(bools[2]){
				electric_field_test_value = electric_field_test;
			}else{
				electric_field_test_value = (double)rho/(coreprof.ne.value.rows()-1.0)*electric_field_test;
			}
			
			//if (bools[0]){
			if(bools[1]){
				//output = 98765.4321;				
				//if (bools[1]){				
				if(bools[0]){
					critical_field = calculate_critical_field(it->electron_density, it->electron_temperature);
					coreprof.profiles1d.eparallel.value(rho) = electric_field_test*critical_field;		
				output = .90119;			
				} else {
					dreicer_field = calculate_dreicer_field(it->electron_density, it->electron_temperature);
					coreprof.profiles1d.eparallel.value(rho) = electric_field_test*dreicer_field;	
					
				output = .90109;				
				}
				
			} else {
			
			//! absolut electric field
				//output = 12345.6789;//coreprof.profiles1d.eparallel.value(rho);
				coreprof.profiles1d.eparallel.value(rho) = electric_field_test;
				output = .90009;
			}
			
			rho++;
		
		}	
		
		output += (double)swint;	
		//rho = pro.size();
		//output += (rho*10);
		
		//! successful running: return 0
		//output = 0;
		
		/*if(electric_field_switch==2){
			output = (double)pro.size();
		}
		
		if(electric_field_switch==3){
			output = (double)coreprof.ne.value.rows();
		}
			
			
		if(electric_field_switch==4){
			output = (double)4;
		}*/	
		
		std::cerr << std::endl << "BOOL 0 : " << bools[0] << std::endl;
		std::cerr << "BOOL 1 : " << bools[1] << std::endl << std::endl;
		std::cerr << "BOOL 2 : " << bools[2] << std::endl << std::endl;
		
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		output = ITM_ILLEGAL_INT;
		
		//! internal error in distribution
		
	}

}
