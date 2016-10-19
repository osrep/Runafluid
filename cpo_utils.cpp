#include <stdexcept>
//#include "cpo_utils.h"
#include <UALClasses.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "constants.h"
#include "distinit.h"
#include "control.h"


/*!
\param a

\details
\f[
	\mathrm{sign}(a) 
\f]
*/

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


/*!
\param a
\param b
\param tolerance

\details
\f[
	2 \cdot \left| a-b\right| \le \left( |a| + |b| \right) \cdot tolerance
\f]
*/
bool equal(double a, double b, double tolerance) {
	return abs(a - b) * 2.0 <= (abs(a) + abs(b)) * tolerance;
}



//! Binary search 

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


/*!
linear interpolation

\f[
	y_a = y_i + \frac{y_{i+1}-y_i}{x_{i+1}-x_i}\cdot(x_a -x_i)
\f]

*/
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

/*!

switch handling

*/

int bool_switch(int switch_number, bool *bools, int N){
	
	
	for (int i=0; i<N; i++){
		if (switch_number%10==1){
			bools[i]=true;
		}else{		
			bools[i]=false;
		}
				
		switch_number /= 10;
	}
	
	return N;

}



int get_digit(int number, int digit){

	int number2,number3;
	for (int i = 0; i < digit; i++){
		number2 = number/10;
		number3 = number - 10*number2;		
		number = number2; 		
	}
	
	
	return number3;
}




int runafluid_switch_message(int runafluid_switch){
	
			
	int	modulevar_rates = get_digit(runafluid_switch,1);
	int modulevar_dreicer = get_digit(runafluid_switch,2);
	int	modulevar_avalanche = get_digit(runafluid_switch,3);	
	int	modulevar_toroidicity = get_digit(runafluid_switch,4);	
	int	modulevar_5 = get_digit(runafluid_switch,5);
	int dreicer_formula_id = 63;
		
			
	std::cerr << "  [Runaway Fluid] Warning: A new Runaway_Fluid actor released where runafluid_switch changed. Please read documentation about how to use runafluid_switch!"<< std::endl;	
	std::cerr << "\t\t\tMore info:\thttp://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runafluid-usermanual"<< std::endl;	
	
	
	
	//! choose Dreicer module scenario
	if (modulevar_dreicer==1) {dreicer_formula_id = 63;}
	if (modulevar_dreicer==2) {dreicer_formula_id = 66;}
	if (modulevar_dreicer==3) {dreicer_formula_id = 67;}	
	
	
	if (modulevar_dreicer == 0){		
		std::cerr << "  [Runaway Fluid] \tDreicer module OFF"<< std::endl;	
	}else{
		std::cerr << "  [Runaway Fluid] \tDreicer module ON"<< std::endl;	
		std::cerr << "\t\t\twith H&C (" << dreicer_formula_id << ") formula"<< std::endl;	
		if (dreicer_formula_id != 63){
			std::cerr << "\t\t\tPlease use formula (63)!"<< std::endl;
			std::cerr << "\t\t\tMore info:\thttp://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runafluid-usermanual"<< std::endl;	
		}
	}
		
	if (modulevar_avalanche == 0){		
		std::cerr << "  [Runaway Fluid] \tAvalanche OFF"<< std::endl;	
	}else{
		std::cerr << "  [Runaway Fluid] \tAvalanche module ON (modulevar: " << modulevar_avalanche << ")" << std::endl;		
	}
	
	
	
}




/*
int bool_switch(bool *bools, int N){
	
	int switch_number = 0;
	for (int i=0; i<N; i++){
		switch_number *= 10;
		if (bools[i]=true){			
			switch_number += 1;
		}
	}
	
	return switch_number;

}*/



/*! This code is looking for
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

			//! Is the distribution flag the runaway DISTSOURCE_IDENTIFIER (7)?
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
	
	if (runaway_index == -1){
		std::cerr << "  [Runaway Fluid] WARNING: There is no previous runaway distribution. New distribution initialised." << std::endl;
	}else{	
		std::cerr << "  [Runaway Fluid] Distri_vec identifier: " << runaway_index << std::endl;
	}
	
	return runaway_index;

}




/*!

Copy data from coreprof CPO
Other fields would be undefined!

input: ItmNs::Itm::coreprof &coreprof
output: profile

*/

profile read_coreprof(const ItmNs::Itm::coreprof &coreprof) {

	profile pro;

	//! read electron density profile length of dataset: cell_length	
	int cell_length = coreprof.ne.value.rows();
	
	//! read electron temperature profile length of dataset, comparing with cell_length
	if (coreprof.te.value.rows() != cell_length)
		throw std::invalid_argument("  [Runaway Fluid] Number of values is different in coreprof ne and te.");

	//! read eparallel profile length of dataset, comparing with cell_length
	if (coreprof.profiles1d.eparallel.value.rows() != cell_length)
		throw std::invalid_argument(
				"  [Runaway Fluid] Number of values is different in coreprof.ne and coreprof.profiles1d.eparallel.");

    //! read data in every $\rho$ 

	for (int i = 0; i < cell_length; i++) {
		cell celll;
				
		//! electron density
		celll.electron_density = coreprof.ne.value(i);
		
		//! electron temperature
		celll.electron_temperature = coreprof.te.value(i);
		
		/*! local electric field
			\f[ E = E_\parallel(\rho) 
			where $\rho$ normalised minor radius
		*/
		celll.electric_field = coreprof.profiles1d.eparallel.value(i); 

		pro.push_back(celll);
	}

	return pro;
}


profile read_coreprof_equilibrium(const ItmNs::Itm::coreprof &coreprof,const ItmNs::Itm::equilibrium &equilibrium) {

	profile pro;

	//! read electron density profile length of dataset: cell_length	
	int cell_length = coreprof.ne.value.rows();
	
	//! read electron temperature profile length of dataset, comparing with cell_length
	if (coreprof.te.value.rows() != cell_length)
		throw std::invalid_argument("  [Runaway Fluid] Number of values is different in coreprof ne and te.");

	//! read eparallel profile length of dataset, comparing with cell_length
	if (coreprof.profiles1d.eparallel.value.rows() != cell_length)
		throw std::invalid_argument(
				"  [Runaway Fluid] Number of values is different in coreprof.ne and coreprof.profiles1d.eparallel.");

    //! read data in every $\rho$ 

	for (int i = 0; i < cell_length; i++) {
		cell celll;
				
		//! electron density
		celll.electron_density = coreprof.ne.value(i);
		
		//! electron temperature
		celll.electron_temperature = coreprof.te.value(i);
		
		/*! local electric field
			\f[ E = \frac{E_\parallel(\rho) B_0}{B_\mathrm{av}(\rho)} \f]
			where B_\mathrm{av} is known on discreate \f$R \f$ major radius and interpolated at $\rho$ normalised minor radius
		*/
	
		celll.electric_field = coreprof.profiles1d.eparallel.value(i) * coreprof.toroid_field.b0
				/ interpolate(equilibrium.profiles_1d.rho_tor, equilibrium.profiles_1d.b_av,
						coreprof.rho_tor(i));					
	

		pro.push_back(celll);
	}

	return pro;
}


/*!

Copy data from CPO inputs to profile structure

*/
		
profile cpo_to_profile(const ItmNs::Itm::coreprof &coreprof, const ItmNs::Itm::coreimpur &coreimpur,
		const ItmNs::Itm::equilibrium &equilibrium, const ItmNs::Itm::distribution &distribution){		

	profile pro;
	double number_of_parts;

	//! read electron density profile length of dataset: cell_length	
	int cell_length = coreprof.ne.value.rows();
	
	//! read electron temperature profile length of dataset, comparing with cell_length
	if (coreprof.te.value.rows() != cell_length){
	//	std::cerr << "ERROR : Number of values is different in CPOPROFILE\tne: " << cell_length << " and Te: " << coreprof.te.value.rows() << std::endl;
		throw std::invalid_argument("  [Runaway Fluid] Number of values is different in coreprof ne and Te.");		
	}		
	
	//! read eparallel profile length of dataset, comparing with cell_length
	if (coreprof.profiles1d.eparallel.value.rows() != cell_length){
	//	std::cerr << "ERROR : Number of values is different in CPOPROFILE\tne: " << cell_length << " and Eparallel: " << coreprof.profiles1d.eparallel.value.rows() << std::endl;		
		throw std::invalid_argument(
				"  [Runaway Fluid] Number of values is different in coreprof.ne and coreprof.profiles1d.eparallel.");		
	}			
													
						
	//! read distribution source index for runaways from distribution CPO						
	int distsource_index = whereRunaway(distribution);	
								
    //! read data in every $\rho$ 

	for (int i = 0; i < cell_length; i++) {
		cell celll;
		
		//! normalised minor radius
		celll.rho = coreprof.rho_tor_norm(i);
				
		//! electron density
		celll.electron_density = coreprof.ne.value(i);
		
		//! electron temperature
		celll.electron_temperature = coreprof.te.value(i);
		
		/*! local electric field
			\f[ E = \frac{E_\parallel(\rho) B_0}{B_\mathrm{av}(\rho)} \f]
			where B_\mathrm{av} is known on discreate \f$R \f$ major radius and interpolated at $\rho$ normalised minor radius
		*/
		try{
			celll.electric_field = coreprof.profiles1d.eparallel.value(i) * coreprof.toroid_field.b0
					/ interpolate(equilibrium.profiles_1d.rho_tor, equilibrium.profiles_1d.b_av,
							coreprof.rho_tor(i));
		
		//! internal error in equilibrium
		} catch (const std::exception& ex) {
			celll.electric_field = 0;			
			std::cerr << "  [Runaway Fluid] ERROR : rho_tor is empty in equilibrium CPO, electric field set to zero. (" << i << ")" << std::endl;
		}
		
		try{		
			//! local magnetic field
			celll.magnetic_field = interpolate(equilibrium.profiles_1d.rho_tor, equilibrium.profiles_1d.b_av,
							coreprof.rho_tor(i));
			
		} catch (const std::exception& ex) {
			celll.magnetic_field = 0;			
			std::cerr << "  [Runaway Fluid] ERROR : in magnetic field, magnetic field set to zero. (" << i << ")" << std::endl;
		}
		
		try{		
			//! No runaway in previous distribution CPO
			if (distsource_index<0){
				celll.runaway_density = 0;
			//! Runaway in previous distribution CPO
			}else{
				celll.runaway_density = distribution.distri_vec(distsource_index).profiles_1d.state.dens(i);
			}

		//! internal error in distribution
		} catch (const std::exception& ex) {

			celll.runaway_density = 0;
			
			std::cerr << "  [Runaway Fluid] WARNING : Cannot read runaway density, density set to zero." << std::endl;
		}

		//! total sum of electric charge from coreprof CPO
		celll.effective_charge = coreprof.profiles1d.zeff.value(i);	

		pro.push_back(celll);
	}

	return pro;
}		
