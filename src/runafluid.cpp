#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>
#include <libxml/xmlreader.h>
#include "DecodeITMpar.h"		
#include <unistd.h>
#include "H5Cpp.h"
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#include <cassert>
#include <array>

#include "runafluid.h"
#include "codeparams.h"
#include "constants.h"
#include "cpo_utils.h"
#include "distinit.h"
#include "control.h"
#include "codeparams.h"
#include "hdf5export.h"
#include "plasma_structures.h"
using namespace H5;
using namespace std;

/*

main function

*/

// HDF5 init
int create_hdf5 (void);


void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		  ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution_in,
		  ItmNs::Itm::distribution &distribution_out, double &timestep,
		  int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  int &shot_number, int &run_number, ItmNs::codeparam_t &codeparams) {

	//!start: runafluid
	std::cout << " START: runaway_fluid" << std::endl;

	// parse codeparam
	module_struct modules = read_codeparams(codeparams);

	// get time
	double time = coreprof.time;

	// output initialisation
	runaway_warning = 0;
	not_suitable_warning = 0;
	critical_fraction_warning = 0;

	// zero limit
	double zero_threshold = 1e-20;

	// maximal normalised minor radius
	double rho_max = modules.rho_edge_calculation_limit;

	// critical fraction
	double critical_fraction = modules.warning_percentage_limit;

	// empty distribution initialiser (integrated distinit)
	distinit(distribution_out, coreprof);

	// Number of elements in runaway electron distribution
	int N_rho = coreprof.ne.value.rows();

	// runaway density and current initialisation
	double rundensity = 0.0;
	double runcurrent = 0.0;

	// electron current initialisation
	double ecurrent = 0.0;

	// reading profile from CPO inputs (cpo_utils.h)
	plasma_profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution_in); // testing until previous distribution validating

	// stepping iterator in profile
	int rho_index = 0;

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
	double inv_asp_ratio = equilibrium.eqgeometry.a_minor / coreprof.toroid_field.r0;

	// Runaway fluid switch message
	list_parameter_settings(modules);
	bool hdf5_switch = true;

	// Distribution source index for output
	int distsource_out_index = 0;

	for (plasma_profile::iterator it = pro.begin(); it != pro.end(); ++it) {
			
		// Length of the runaway distribution is correct
		if (rho_index<distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows()){
			it->electric_field = std::abs(it->electric_field);
			it->magnetic_field = std::abs(it->magnetic_field);
			// calculating runaway density
			rundensity = advance_runaway_population(*it,timestep, inv_asp_ratio, it->rho, modules, rate_values);

			// no runaway if is larger then a preset maximum rho	
		   	if (it->rho >= rho_max){
				rundensity = 0;
			}
			  
		   	// CPO output -- runaway warning
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
		   	
		   	// runaway density
	   		distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens(rho_index) = rundensity;
		  
		   	// runaway current
		   	runcurrent = rundensity * ITM_QE * ITM_C * sign(it->electric_field);
		   	distribution_out.distri_vec(distsource_out_index).profiles_1d.state.current(rho_index) = runcurrent;
		   	
		   	// not suitable warning: j_R > j_e	
		   	ecurrent = it->electron_density * ITM_QE * ITM_C * sign(it->electric_field);
		   	if (runcurrent/ecurrent >= 1){
				not_suitable_warning = 1;
		   	}
		   	
			dreicer_prof[rho_index]        = rate_values[RATEID_DREICER];
			avalanche_prof[rho_index]      = rate_values[RATEID_AVALANCHE];
			electric_field_prof[rho_index] = rate_values[RATEID_ELECTRIC_FIELD];
			critical_field_prof[rho_index] = rate_values[RATEID_CRITICAL_FIELD];
	   		
	   	}else{
			std::cerr << "  [Runaway Fluid] \tERROR: The length of runaway distribution array is incorrect(" << rho_index << "/"
					  << distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows() << ")" << std::endl;
	   	}

	    rho_index++;
	
	}

   	// error messages to dump
   	if (runaway_warning == 1){
		std::cout << "  [Runaway Fluid] \tWarning: Runaway electrons detected at " << time << " s" << std::endl;
		output_flag = 1;
	}
		
	if (not_suitable_warning == 1){
		std::cout << "  [Runaway Fluid] \tWarning: Runaway current is higher than electron current at " << time << " s" << std::endl;
		output_flag = 2;
	}
	
	if (critical_fraction_warning == 1){
		std::cout << "  [Runaway Fluid] \tWarning: Runaway density is higher than the range of validity (critical fraction: "
				  << critical_fraction << "%)  at " << time << " s" << std::endl;
		output_flag = 3;
	}

	// output flag to distribution CPO
	try {
		distribution_out.codeparam.output_flag = output_flag;

		if (output_flag == 1){
			distribution_out.codeparam.output_diag = "Runaway Fluid was running successfully and runaway electrons indicated";
		}
		else if (output_flag == 2){
			distribution_out.codeparam.output_diag = "Runaway Fluid was running successfully but runaway current is higher than electron current";
		}
		else if (output_flag == 3){
			distribution_out.codeparam.output_diag = "Runaway Fluid was running successfully but results out of the range of validity";
		}
		
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] \tERROR: An error occurred during filling output_flag in codeparam" << std::endl;
		std::cerr << "  [Runaway Fluid] \tERROR: " << ex.what() << std::endl;
	}

	// HDF5 export
	if (modules.hdf5_output == true){

			const char *hdf5_base;

			if ((hdf5_base = getenv("HDF5_BASE")) == NULL){

				const char *homedir;

				if ((homedir = getenv("HOME")) == NULL) {

					homedir = getpwuid(getuid())->pw_dir;
					hdf5_base = homedir;

				} else hdf5_base = getenv("HOME");

			} else hdf5_base = getenv("HDF5_BASE");

			std::string str_shot_number = int_to_string(shot_number);

			char char_run_number [4];
			sprintf(char_run_number, "%04i", run_number);

			std::string filename = "/euitm_" + str_shot_number + char_run_number + "_runafluid" + ".h5";
			std::string hdf5_file_name = hdf5_base + filename;

			const int dataset_name_length = 14; 
			string dataset_name_list[dataset_name_length] = {
				"time","rho_tor","rho_tor_eq",
				"density", "temperature", "eparallel","b0", "zeff",
				"runaway_density","runaway_current","dreicer_rate","avalanche_rate",
				"electric_field_vs_critical_field", "critical_field"};
			int cols = rho_index;//sizeof dataext / sizeof(double);
			if (init_hdf5_file(hdf5_file_name,cols,dataset_name_list, dataset_name_length)==0){

				//write_data_to_hdf5(hdf5_file_name, "time", time);
				//write_data_to_hdf5(hdf5_file_name, "rho_tor", coreprof.rho_tor);
				//write_data_to_hdf5(hdf5_file_name, "rho_tor_eq", equilibrium.profiles_1d.rho_tor);				
				//write_data_to_hdf5(hdf5_file_name, "density", coreprof.ne.value);
				//write_data_to_hdf5(hdf5_file_name, "temperature", coreprof.te.value);
				//write_data_to_hdf5(hdf5_file_name, "eparallel", coreprof.profiles1d.eparallel.value);
				//write_data_to_hdf5(hdf5_file_name, "b0", coreprof.toroid_field.b0);
				//write_data_to_hdf5(hdf5_file_name, "zeff", coreprof.profiles1d.zeff.value);
				//write_data_to_hdf5(hdf5_file_name, "runaway_density", distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens);		
				//write_data_to_hdf5(hdf5_file_name, "runaway_current", distribution_out.distri_vec(distsource_out_index).profiles_1d.state.current);
				//write_data_to_hdf5(hdf5_file_name, "dreicer_rate", dreicer_prof);
				//write_data_to_hdf5(hdf5_file_name, "avalanche_rate", avalanche_prof);
				//write_data_to_hdf5(hdf5_file_name, "electric_field_vs_critical_field", electric_field_prof);
				//write_data_to_hdf5(hdf5_file_name, "critical_field", critical_field_prof);
				
			}else{
				cout << "  [Runaway Fluid] \tHDF5 init was not successful." << endl;
			}

	}

	// end: runafluid
	std::cout << " END: runaway_fluid" << std::endl;
}

std::string int_to_string( int x ) {
  // Introduce the int_to_string function, std::to_string didn't work with gcc 4.8.5
  int length = snprintf( NULL, 0, "%d", x );
  assert( length >= 0 );
  char* buf = new char[length + 1];
  snprintf( buf, length + 1, "%d", x );
  std::string str( buf );
  delete[] buf;
  return str;
}
