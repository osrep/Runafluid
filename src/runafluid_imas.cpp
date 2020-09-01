#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>
#include <libxml/xmlreader.h>
#include "DecodeITMpar.h"
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#include <cassert>
#include <array>

#include "runafluid_imas.h"
#include "constants.h"
#include "ids_utils.h"
#include "control.h"
#include "codeparams_imas.h"
#include "distinit.h"
#include "hdf5export.h"
#include "plasma_structures.h"
#include "H5Cpp.h"
using namespace H5;
using namespace std;
/*

main function

fix time label

*/

void fire(IdsNs::IDS::core_profiles &core_profiles,
		  IdsNs::IDS::equilibrium &equilibrium, IdsNs::IDS::distributions &distribution_in,
		  IdsNs::IDS::distributions &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  IdsNs::IDS::temporary &runaway_rates) {

	// start: runafluid
	std::cerr << " START: runaway_fluid" << std::endl;
	
	// parse codeparam
	module_struct modules = read_codeparams(codeparams);
	
	// get time
	int timeindex = 0;
	
	// output initialisation
	runaway_warning = 0;
	not_suitable_warning = 0;
	critical_fraction_warning = 0;
	
	// zero limit
	double zero_threshold = 1e-20;
	
	// maximal normalised minor radius
	double rho_max = 0.95;
	
	// empty distribution initialiser (integrated distinit)
	distinit(distribution_out, core_profiles, 0);
	
	// Number of elements in runaway electron distribution
	int N_rho = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();

	// runaway density and current initialisation	
	double rundensity = 0.0;
	double runcurrent = 0.0;
	
	// electron current initialisation
	double ecurrent = 0.0;
	
	// reading profile from IDS inputs (ids_utils.h)
	plasma_profile pro = ids_to_profile(core_profiles, equilibrium, distribution_in, timeindex); // testing until previous distribution validating

	// stepping iterator in profile
	int i = 0;
	
	// output flag
	int output_flag = 0;

	// Number of rate calculations (Dreicer, Avalanche etc.)
	double rate_values[N_RATES];
	int pro_size = pro.size();
	std::vector<double> dreicer_prof(pro_size);
	std::vector<double> avalanche_prof(pro_size);
	std::vector<double> electric_field_prof(pro_size);
	std::vector<double> critical_field_prof(pro_size);
	
	// inverse aspect ratio
	double inv_asp_ratio = equilibrium.time_slice(timeindex).boundary.minor_radius / equilibrium.vacuum_toroidal_field.r0;
	
	// Runaway fluid switch message
	list_parameter_settings(modules);

	// Distribution source index for output
	int distsource_out_index = 0;
	
	for (std::vector<plasma_local>::iterator it = pro.begin(); it != pro.end(); ++it) {

		// Length of the runaway distribution is correct
		if (i<distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).density.rows()){

			// calculating runaway density
			rundensity = advance_runaway_population(*it, timestep, inv_asp_ratio, it->rho, modules, rate_values);

			// no runaway if  rho is larger then a preset maximum rho value
		   	if (it->rho >= rho_max){
				rundensity = 0;
			}

		   	// IDS output -- runaway warning
	   		if (rundensity > zero_threshold){
				runaway_warning = 1;
			}else{
				rundensity = 0; // no runaway
			}
	
			//  critical fraction warning
	   		if (rundensity > critical_fraction/100.0*it->electron_density){
				critical_fraction_warning = 1;
			}

		   	// runaway density hard limit
		   	if (rundensity > it->electron_density){
		   		rundensity = it->electron_density;
		   	}

		   	// runaway density writing to output distribution
	   		distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).density(i) = rundensity;

		   	// runaway current
		   	runcurrent = rundensity * ITM_QE * ITM_C * sign(it->electric_field);
		   	distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).current_tor(i) = runcurrent;

		   	// not suitable warning: j_R > j_e	
		   	ecurrent = it->electron_density * ITM_QE * ITM_C * sign(it->electric_field);
		   	if (runcurrent/ecurrent >= 1){
				not_suitable_warning = 1;
		   	}

			dreicer_prof[i]        = rate_values[RATEID_DREICER];
			avalanche_prof[i]      = rate_values[RATEID_AVALANCHE];
			electric_field_prof[i] = rate_values[RATEID_ELECTRIC_FIELD];
			critical_field_prof[i] = rate_values[RATEID_CRITICAL_FIELD];

	   	}else{
			std::cerr << "  [Runaway Fluid] ERROR: The length of runaway distribution array is incorrect(" << i << "/"
					  << distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).density.rows() << ")" << std::endl;
	   	}

	    i++;

	}

   	// error messages to dump
   	if (runaway_warning == 1){
		std::cerr << "  [Runaway Fluid] Warning: Runaway electrons detected at " << time << " s" << std::endl;
		output_flag = 1;
	}

	if (not_suitable_warning == 1){
		std::cerr << "  [Runaway Fluid] Warning: Runaway current is higher than electron current at " << time << " s" << std::endl;
		output_flag = 2;
	}

	if (critical_fraction_warning == 1){
		std::cerr << "  [Runaway Fluid] Warning: Runaway density is higher than the range of validity (critical fraction: "
				  << critical_fraction << "%)  at " << time << " s" << std::endl;
		output_flag = 3;
	}

	// output flag to distribution CPO
	try {
		distribution_out.code.output_flag(timeindex) = output_flag;

	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during filling output_flag in codeparam" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}

	distribution_out.time(timeindex) = distribution_in.time(timeindex)+timestep;

	// end: runafluid
	std::cerr << " END: runaway_fluid" << std::endl;
}

module_struct init_modules_from_runafluid_switch(int runafluid_switch){
	module_struct modules;
	switch (get_digit(runafluid_switch, 4)){
		case 1:  modules.dreicer_toroidicity = true;  modules.avalanche_toroidicity = true;  break;
		case 2:  modules.dreicer_toroidicity = true;  modules.avalanche_toroidicity = false; break;
		case 3:  modules.dreicer_toroidicity = false; modules.avalanche_toroidicity = true;  break;
		default: modules.dreicer_toroidicity = false; modules.avalanche_toroidicity = false; break;
	}

	switch (get_digit(runafluid_switch, 3)){
		case 1:  modules.avalanche_formula.assign("rosenbluth_putvinski_with_threshold"); break;
		case 3:  modules.avalanche_formula.assign("rosenbluth_putvinski"); break;
	}

	switch (get_digit(runafluid_switch, 2)){
		case 1:  modules.dreicer_formula.assign("hc_formula_63"); break;
		case 2:  modules.dreicer_formula.assign("hc_formula_66"); break;
		case 3:  modules.dreicer_formula.assign("hc_formula_67"); break;
	}

	modules.output_path.assign("default.h5");
	
	return modules;
}
