#include <stdexcept>
#include "ids_utils.h"


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

double interpolate(const Array<double, 1> &x, const Array<double, 1> &y, double xa) {

	int rows = x.rows();
	if (rows != y.rows())
		throw std::invalid_argument("Number of rows is different in arrays to interpolate.");

	if (xa <= x(0))
		return y(0);
	if (xa >= x(x.rows() - 1))
		return y(y.rows() - 1);

	int index = binary_search(x, xa);
	if (index < 0)
		throw std::invalid_argument("Binary search failed.");

	return y(index) + (y(index + 1) - y(index)) / (x(index + 1) - x(index)) * (xa - x(index));
}


/*! This code is looking for
which element of distri_vec is 
the runaway distribution

runaway: distri_vec index
no runaway: -1

*/
int whereRunaway(const IdsNs::IDS::distributions &distributions){
	int N_distr = 0;
	
	int runaway_index = -1;
	
	try {
		N_distr = distributions.distribution.rows();
		
		for (int i=0; (i<N_distr && runaway_index<0); i++){

			// Is the distribution flag the runaway DISTSOURCE_IDENTIFIER (7)?
			if (distributions.distribution(i).process.rows()>0){
				if (distributions.distribution(i).process(0).type.index == DISTSOURCE_IDENTIFIER){
					runaway_index = i;
				}
			}
		}
		
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR : An error occurred during geometry vectors allocation" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR : " << ex.what() << std::endl;
	}
	
	if (runaway_index == -1){
		std::cerr << "  [Runaway Fluid] WARNING: There is no previous runaway distribution." << std::endl;
	}else{	
		std::cerr << "  [Runaway Fluid] Distri_vec identifier: " << runaway_index << std::endl;
	}
	
	return runaway_index;

}


// if rho_tor_norm empty, we need to fill it up

double fill_rho_tor_norm(const IdsNs::IDS::core_profiles &core_profiles, const IdsNs::IDS::equilibrium &equilibrium, int cpindex, int timeindex){
    
    double rho_tor_norm;
    
	int N_rho_tor = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();
	int N_rho_tor_norm = core_profiles.profiles_1d(timeindex).grid.rho_tor_norm.rows();

    if (N_rho_tor>N_rho_tor_norm){
        rho_tor_norm = interpolate(equilibrium.time_slice(timeindex).profiles_1d.rho_tor,
            equilibrium.time_slice(timeindex).profiles_1d.rho_tor_norm,
		    core_profiles.profiles_1d(timeindex).grid.rho_tor(cpindex));
    }else{
        rho_tor_norm = core_profiles.profiles_1d(timeindex).grid.rho_tor_norm(cpindex);
    }

	return rho_tor_norm;

}


// IMAS utilities
// https://portal.iter.org/departments/POP/CM/IMDesign/Data%20Model/CI/imas-3.7.3/html_documentation.html
plasma_profile ids_to_profile(const IdsNs::IDS::core_profiles &core_profiles, int timeindex){

	plasma_profile pro;

	// read electron density profile length of dataset: cells	
	int cells = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();
	
	if (core_profiles.profiles_1d(timeindex).electrons.density.rows() != cells)
		throw std::invalid_argument("Number of values is different in coreprof rho cordinates and electron density.");

	// read electron temperature profile length of dataset, comparing with N_rho
	if (core_profiles.profiles_1d(timeindex).electrons.temperature.rows() != cells)
		throw std::invalid_argument("Number of values is different in coreprof rho cordinates and electron temperature.");

	// read eparallel profile length of dataset, comparing with N_rho
	if (core_profiles.profiles_1d(timeindex).e_field.parallel.rows() != cells)
		throw std::invalid_argument(
				"Number of values is different in coreprof rho coordinates and eparallel.");	


	// read data in every rho
	for (int i = 0; i < cells; i++) {
		plasma_local plasmaLocal;
		
		// electron density
		plasmaLocal.electron_density = core_profiles.profiles_1d(timeindex).electrons.density(i);
		
		// electron temperature
		plasmaLocal.electron_temperature = core_profiles.profiles_1d(timeindex).electrons.temperature(i);
						
		// total sum of electric charge in a rho cell
		plasmaLocal.effective_charge = core_profiles.profiles_1d(timeindex).zeff(i);

		pro.push_back(plasmaLocal);
	}

	return pro;
}


plasma_profile ids_to_profile(const IdsNs::IDS::core_profiles &core_profiles, const IdsNs::IDS::equilibrium &equilibrium, const IdsNs::IDS::distributions &distributions, int timeindex){

	plasma_profile pro;

	// read electron density profile length of dataset: N_rho
	int N_rho_tor = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();
	int N_rho_tor_norm = core_profiles.profiles_1d(timeindex).grid.rho_tor_norm.rows();
	
	int N_rho = (N_rho_tor>N_rho_tor_norm)?N_rho_tor:N_rho_tor_norm;
	
    	// read distribution source index for runaways from distribution CPO
	int distsource_index = whereRunaway(distributions);	

    	// read data in every rho
	for (int i = 0; i < N_rho; i++) {
		plasma_local plasmaLocal;
		// normalised minor radius
		plasmaLocal.rho = fill_rho_tor_norm(core_profiles, equilibrium, i, timeindex);
		
		// electron density
		plasmaLocal.electron_density = core_profiles.profiles_1d(timeindex).electrons.density(i);
		
		// electron temperature
		plasmaLocal.electron_temperature = core_profiles.profiles_1d(timeindex).electrons.temperature(i);
		
		// parallel electric field
		plasmaLocal.electric_field = core_profiles.profiles_1d(timeindex).e_field.parallel(i);
						
		// local magnetic field
		plasmaLocal.magnetic_field = interpolate(equilibrium.time_slice(timeindex).profiles_1d.rho_tor, equilibrium.time_slice(timeindex).profiles_1d.b_field_average,
						core_profiles.profiles_1d(timeindex).grid.rho_tor(i));
				
		// total sum of electric charge in a rho cell
		plasmaLocal.effective_charge = core_profiles.profiles_1d(timeindex).zeff(i);
		
		try{		
			// No runaway in previous distribution CPO
			if (distsource_index<0){
				plasmaLocal.runaway_density = 0;
			// Runaway in previous distribution CPO
			}else{
				plasmaLocal.runaway_density = distributions.distribution(distsource_index).profiles_1d(timeindex).density(i);
			}

		// internal error in distribution
		} catch (const std::exception& ex) {

			plasmaLocal.runaway_density = 0;
			
			std::cerr << "  [Runaway Fluid] WARNING : Cannot read runaway density, density set to zero." << std::endl;
		}

		pro.push_back(plasmaLocal);
	}

	return pro;
}
