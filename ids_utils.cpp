#include <stdexcept>
#include "ids_utils.h"

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


int whereRunaway(const ItmNs::Itm::distributions &distributions){
	int N_distr = 0;
	
	int runaway_index = -1;
	
	try {
		N_distr = distributions.distribution.rows();
		
		for (int i=0; (i<N_distr && runaway_index<0); i++){

			//! Is the distribution flag the runaway DISTSOURCE_IDENTIFIER (7)?
			if (distributions.distribution(i).process()>0){
				if (distributions.distribution(i).process(0).type == DISTSOURCE_IDENTIFIER){/process(i2)/type
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


// IMAS utilities
// https://portal.iter.org/departments/POP/CM/IMDesign/Data%20Model/CI/imas-3.7.3/html_documentation.html
profile ids_to_profile(const IdsNs::IDS::core_profiles &core_profiles, const IdsNs::IDS::equilibrium &equilibrium, const IdsNs::IDS::distributions &distributions, int timeindex){

	profile pro;

	//! read electron density profile length of dataset: cells	
	int cells = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();
	
    //! read distribution source index for runaways from distribution CPO						
	int distsource_index = whereRunaway(distributions);	
	
    //! read data in every $\rho$ 
	for (int rho = 0; rho < cells; rho++) {
		cell celll;
		//! normalised minor radius
		celll.rho = core_profiles.profiles_1d(timeindex).grid.rho_tor_norm(i);
		
		//! electron density
		celll.electron_density = core_profiles.profiles_1d(timeindex).electrons.density(rho);
		
		//! electron temperature
		celll.electron_temperature = core_profiles.profiles_1d(timeindex).electrons.temperature(rho);
		
		/*! local electric field
			\f[ E = \frac{E_\parallel(\rho) B_0}{B_\mathrm{av}(\rho)} \f]
			where B_\mathrm{av} is known on discreate \f$R \f$ major radius and interpolated at $\rho$ normalised minor radius
		*/
				
		celll.electric_field = core_profiles.profiles_1d(timeindex).e_field.parallel(rho) * core_profiles.vacuum_toroidal_field.b0(timeindex)
				/ interpolate(equilibrium.time_slice(timeindex).profiles_1d.rho_tor, equilibrium.time_slice(timeindex).profiles_1d.b_field_average,
						core_profiles.profiles_1d(timeindex).grid.rho_tor(rho));
						
		//! total sum of electric charge in \a rho cell
		celll.effective_charge = core_profiles.profiles_1d(timeindex).zeff(rho);
		
		try{		
			//! No runaway in previous distribution CPO
			if (distsource_index<0){
				celll.runaway_density = 0;
			//! Runaway in previous distribution CPO
			}else{
				celll.runaway_density = distribution.distribution(distsource_index).profiles_1d(timeindex).density(i);
			}

		//! internal error in distribution
		} catch (const std::exception& ex) {

			celll.runaway_density = 0;
			
			std::cerr << "  [Runaway Fluid] WARNING : Cannot read runaway density, density set to zero." << std::endl;
		}

		pro.push_back(celll);
	}

	return pro;
}
