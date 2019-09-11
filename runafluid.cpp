#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>
#include <libxml/xmlreader.h>
#include "DecodeITMpar.h"		
#include <unistd.h>
#include "H5Cpp.h"

#include "runafluid.h"
#include "codeparams.h"
#include "constants.h"
#include "cpo_utils.h"
#include "distinit.h"
#include "control.h"
#include "codeparams.h"
#include "hdf5export.h"
using namespace H5;
using namespace std;


/*! \mainpage

Purpose

This document specifies software requirements of the Runaway Fluid (runafluid) software module.
The module is developed and deployed in the European Transport Solver (ETS) framework maintained by the Code Development
for Integrated Modelling Project (ITM) of the EUROfusion consortium (http://portal.efda-itm.eu).

Intended Audience and Reading Suggestions

This Software Requirements Specification (SRS) document is intended for ITM/ETS/H&CD workflow developers,
and developers of runafluid module. The document describes the module runafluid, laying out functional
and non-functional requirements. Purpose, overview of the module, interfaces, and CPO objects used,
constraints, assumptions and dependencies, functional requirements are contained in this documentation.


Product Scope

The Runaway Fluid (Runafluid) project supplies a simulator module assembled as a Kepler workflow actor,
which is capable of indicating whether runaway electron generation is to be expected during tokamak operation.
This functionality is highly valuable in ITM simulations, since present equilibrium and transport calculations neglect
the generation of runaway electrons. The Runafluid module can determine whether runaways are generated thus validate
the results of equilibrium and transport modules in this manner.

\subpage references

*/


/*! \page references References

Basic description is provided at:
http://portal.efda-itm.eu/twiki/bin/view/Main/HCD-ElectronRun-awayPhysics?sso_from=bin/view/Main/HCD-ElectronRun-awayPhysics.

User manual is maintained at http://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runin-usermanual.

Runaway Indicator is maintained under the ITM-TF Collaborative Software Development Environment using Gforge.
The project documentation is accessible via http://gforge.efda-itm.eu/gf/project/runin/. Source code is stored in the
SVN repository https://gforge.efda-itm.eu/svn/runin.

Analytical formula used to determine the critical electric field is based on the work of J.W. Connor and R.J. Hastie [1].
The method of calculating Dreicer runaway generation growth rate stems from the article of H. Dreicer [2].

[1] A. Stahl, E. Hirvijoki, J. Decker, O. Embr�us, and T. F�l�p. Effective Critical Electric Field for Runaway-Electron Generation.
Physical Review Letters 114(11), 115002 (2015)

[2] H. Smith, P. Helander, L.-G. Eriksson, D. Anderson, M. Lisak, and F. Andersson, Runaway electrons and the evolution
of the plasma current in tokamak disruptions,  Physics of Plasmas 13, 102502 (2006)

*/


/*! 

main function


*/

//HDF5 init
int create_hdf5 (void);


void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		  ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution_in,
		  ItmNs::Itm::distribution &distribution_out, double &timestep,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  ItmNs::codeparam_t &codeparams) {

	//! start: runafluid
	std::cout << " START: runaway_fluid" << std::endl;
	
	/*old switch
	if(runafluid_switch!=0){
		std::cout << "  [Runaway Fluid] \tWarning: A new Runaway_Fluid actor released where runafluid_switch removed. Please read documentation about how to use Code Parameters!"
		<< std::endl << "\t\t\tMore info:\thttp://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runafluid-usermanual"<< std::endl;
	}*/


	//! parse codeparam
	modules m = read_codeparams(codeparams);
	runafluid_switch = set_switch_from_codeparams(codeparams);

	//! get time
	double time = coreprof.time;
	
	//! output initialisation
	runaway_warning = 0;
	not_suitable_warning = 0;
	critical_fraction_warning = 0;
	
	//! zero limit
	double zero_threshold = 1e-20;
	
	//! maximal normalised minor radius
	double rho_max = 0.95;
	
	//! empty distribution initialiser (integrated distinit)
	distinit(distribution_out, coreprof);	
	
	//! Number of elements in runaway electron distribution
	int N_rho = coreprof.ne.value.rows();
		
	//! runaway density and current initialisation	
	double rundensity = 0.0;
	double runcurrent = 0.0;
	
	//! electron current initialisation
	double ecurrent = 0.0;		
	
	//! reading profile from CPO inputs (cpo_utils.h)
	profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution_in); // testing until previous distribution validating
		
	//! stepping iterator in profile	
	int rho_index = 0;	
	
	//! output flag
	int output_flag = 0;
	
	//! runaway fluid temporary rates
	int modulevar_rates = 1;//get_digit(runafluid_switch,1);
				
	//! Number of rate calculations (Dreicer, Avalanche etc.)	
	int N_rates = 20;
	double rate_values[N_rates];		
	
	init_rates(runaway_rates, N_rates, N_rho);
	
	//! inverse aspect ratio \eps = a/R
	double inv_asp_ratio = equilibrium.eqgeometry.a_minor / coreprof.toroid_field.r0;
	
	//! Runaway fluid switch message	
	runafluid_switch_message(runafluid_switch);
	bool hdf5_switch = true;

	//! Distribution source index for output
	int distsource_out_index = 0;
	
	for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
		//! Length of the runaway distribution is correct
		if (rho_index<distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows()){
			
			//! calculating runaway density
			rundensity = runafluid_control(it->electron_density, it->runaway_density, it->electron_temperature,
										   it->effective_charge, abs(it->electric_field), abs(it->magnetic_field),
										   timestep, inv_asp_ratio, it->rho, modules, rate_values);
		

			//! no runaway if  \rho \ge \rho_\mathrm{max}			
		   	if (it->rho >= rho_max){
				rundensity = 0;
			}	
			  
		   	//! CPO output -- runaway warning
	   		if (rundensity > zero_threshold){
				runaway_warning = 1;				
			}else{
				rundensity = 0; // no runaway
			}		
				
			//!  critical fraction warning
	   		if (rundensity > critical_fraction/100.0*it->electron_density){
				critical_fraction_warning = 1;
			}
		   	
		   	//! runaway density hard limit
		   	if (rundensity > it->electron_density){
		   		rundensity = it->electron_density;
		   	}
		   	
		   	//! runaway density n_R if \rho < \rho_\mathrm{max}
	   		distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens(rho_index) = rundensity;
		  
		   	//! runaway current
		   	/*!
		   	
		   	j = n_R q_e c \mathrm{sign}(E)
		   	
		   	*/
		    			   	
		   	runcurrent = rundensity * ITM_QE * ITM_C * sign(it->electric_field);		   	
		   	distribution_out.distri_vec(distsource_out_index).profiles_1d.state.current(rho_index) = runcurrent;
		   	
		   	//! not suitable warning: j_R > j_e	
		   	ecurrent = it->electron_density * ITM_QE * ITM_C * sign(it->electric_field);
		   	if (runcurrent/ecurrent >= 1){		   	
				not_suitable_warning = 1;	 	
		   	}
		   	
			dreicer_prof(rho_index)        = rate_values[RATEID_DREICER];
			avalanche_prof(rho_index)      = rate_values[RATEID_AVALANCHE];
			electric_field_prof(rho_index) = rate_values[RATEID_ELECTRIC_FIELD];
			critical_field_prof(rho_index) = rate_values[RATEID_CRITICAL_FIELD];

	   		
	   	}else{		   	
			std::cerr << "  [Runaway Fluid] \tERROR: The length of runaway distribution array is incorrect(" << rho_index << "/"
					  << distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows() << ")" << std::endl;
	   	}   	   	

	    rho_index++;
	
	}

   	//! error messages to dump
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

	//! output flag to distribution CPO
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
	
	distribution_out.time = distribution_in.time+timestep;

	// HDF5 export
	if (!m.output_path.empty()){
			H5std_string hdf5_file_name(m.output_path);

			int dataset_name_length = 14; 
			string dataset_name_list[dataset_name_length] = {
				"time","rho_tor","rho_tor_eq",
				"density", "temperature", "eparallel","b0", "zeff",
				"runaway_density","runaway_current","dreicer_rate","avalanche_rate",
				"electric_field_vs_critical_field", "critical_field"};
			int cols = rho_index;//sizeof dataext / sizeof(double);
			if (init_hdf5_file(hdf5_file_name,cols,dataset_name_list, dataset_name_length)==0){

				write_data_to_hdf5(hdf5_file_name, "time", distribution_in.time);
				write_data_to_hdf5(hdf5_file_name, "rho_tor", coreprof.rho_tor);
				write_data_to_hdf5(hdf5_file_name, "rho_tor_eq", equilibrium.profiles_1d.rho_tor);				
				write_data_to_hdf5(hdf5_file_name, "density", coreprof.ne.value);
				write_data_to_hdf5(hdf5_file_name, "temperature", coreprof.te.value);
				write_data_to_hdf5(hdf5_file_name, "eparallel", coreprof.profiles1d.eparallel.value);
				write_data_to_hdf5(hdf5_file_name, "b0", coreprof.toroid_field.b0);
				write_data_to_hdf5(hdf5_file_name, "zeff", coreprof.profiles1d.zeff.value);
				write_data_to_hdf5(hdf5_file_name, "runaway_density", distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens);		
				write_data_to_hdf5(hdf5_file_name, "runaway_current", distribution_out.distri_vec(distsource_out_index).profiles_1d.state.current);
				write_data_to_hdf5(hdf5_file_name, "dreicer_rate", runaway_rates.timed.float1d(0).value);
				write_data_to_hdf5(hdf5_file_name, "avalanche_rate", runaway_rates.timed.float1d(1).value);
				write_data_to_hdf5(hdf5_file_name, "electric_field_vs_critical_field", runaway_rates.timed.float1d(19).value);
				write_data_to_hdf5(hdf5_file_name, "critical_field", runaway_rates.timed.float1d(19).value);
				
			}else{
				cout << "  [Runaway Fluid] \tHDF5 init was not successful." << endl;
			}
	}

	//! end: runafluid
	std::cout << " END: runaway_fluid" << std::endl;
}

int init_rates(ItmNs::Itm::temporary &runaway_rates, int N_rates, int N_rho){
//! runaway_rates for generation rates
	//! Dreicer generation rate initialisation
	runaway_rates.timed.float1d.resize(N_rates);
	runaway_rates.timed.float1d(0).identifier.id = "dreicer";
	runaway_rates.timed.float1d(0).identifier.flag = 0;
	runaway_rates.timed.float1d(0).identifier.description = "Dreicer generation rate";
	runaway_rates.timed.float1d(0).value.resize(N_rho);	
		
	//! Avalanche generation rate initialisation
	runaway_rates.timed.float1d(1).identifier.id = "avalanche";
	runaway_rates.timed.float1d(1).identifier.flag = 1;
	runaway_rates.timed.float1d(1).identifier.description = "Avalanche generation rate";
	runaway_rates.timed.float1d(1).value.resize(N_rho);
	
	runaway_rates.timed.float1d(2).identifier.id = "dreicer63";
	runaway_rates.timed.float1d(2).identifier.flag = 2;
	runaway_rates.timed.float1d(2).identifier.description = "Dreicer generation rate by Connor et al. (63)";
	runaway_rates.timed.float1d(2).value.resize(N_rho);				
	
	//! Dreicer generation rate initialisation
	runaway_rates.timed.float1d(3).identifier.id = "dreicer66";
	runaway_rates.timed.float1d(3).identifier.flag = 3;
	runaway_rates.timed.float1d(3).identifier.description = "Dreicer generation rate by Connor et al. (66)";
	runaway_rates.timed.float1d(3).value.resize(N_rho);		
			
	runaway_rates.timed.float1d(4).identifier.id = "dreicer67";
	runaway_rates.timed.float1d(4).identifier.flag = 4;
	runaway_rates.timed.float1d(4).identifier.description = "Dreicer generation rate by Connor et al. (67)";
	runaway_rates.timed.float1d(4).value.resize(N_rho);
	
	runaway_rates.timed.float1d(5).identifier.id = "dreicer_prev";
	runaway_rates.timed.float1d(5).identifier.flag = 5;
	runaway_rates.timed.float1d(5).identifier.description = "Dreicer generation rate in the previous timestep";
	runaway_rates.timed.float1d(5).value.resize(N_rho);	

	//! Dreicer field
	runaway_rates.timed.float1d(6).identifier.id = "dreicerf01";
	runaway_rates.timed.float1d(6).identifier.flag = 6;
	runaway_rates.timed.float1d(6).identifier.description = "Dreicer field (formula 1)";
	runaway_rates.timed.float1d(6).value.resize(N_rho);	
	
	//! critical field
	runaway_rates.timed.float1d(7).identifier.id = "criticalf";
	runaway_rates.timed.float1d(7).identifier.flag = 7;
	runaway_rates.timed.float1d(7).identifier.description = "Critical field";
	runaway_rates.timed.float1d(7).value.resize(N_rho);
	
	//! Coulomb logarithm
	runaway_rates.timed.float1d(8).identifier.id = "coulomblog";
	runaway_rates.timed.float1d(8).identifier.flag = 8;
	runaway_rates.timed.float1d(8).identifier.description = "Coulomb logarithm";
	runaway_rates.timed.float1d(8).value.resize(N_rho);
	
	//! electron collision time
	runaway_rates.timed.float1d(9).identifier.id = "thermal_electron_collision_time";
	runaway_rates.timed.float1d(9).identifier.flag = 9;
	runaway_rates.timed.float1d(9).identifier.description = "Thermal electron collision time";
	runaway_rates.timed.float1d(9).value.resize(N_rho);
		
	//! electron collision time
	runaway_rates.timed.float1d(10).identifier.id = "runaway_collision_time";
	runaway_rates.timed.float1d(10).identifier.flag = 10;
	runaway_rates.timed.float1d(10).identifier.description = "Runaway electron collision time";
	runaway_rates.timed.float1d(10).value.resize(N_rho);
	
	//! Avalanche generation rate initialisation
	runaway_rates.timed.float1d(11).identifier.id = "avalanche01";
	runaway_rates.timed.float1d(11).identifier.flag = 11;
	runaway_rates.timed.float1d(11).identifier.description = "Avalanche generation rate (default)";
	runaway_rates.timed.float1d(11).value.resize(N_rho);
	
	//! Avalanche generation rate initialisation
	runaway_rates.timed.float1d(12).identifier.id = "avalanche02";
	runaway_rates.timed.float1d(12).identifier.flag = 12;
	runaway_rates.timed.float1d(12).identifier.description = "Avalanche generation rate (with onset threshold)";
	runaway_rates.timed.float1d(12).value.resize(N_rho);
	
	//! Avalanche generation rate initialisation
	runaway_rates.timed.float1d(13).identifier.id = "avalanche03";
	runaway_rates.timed.float1d(13).identifier.flag = 13;
	runaway_rates.timed.float1d(13).identifier.description = "Avalanche generation rate (linear without threshold)";
	runaway_rates.timed.float1d(13).value.resize(N_rho);
	
	//! Avalanche generation rate initialisation
	runaway_rates.timed.float1d(14).identifier.id = "avalanche_onset";
	runaway_rates.timed.float1d(14).identifier.flag = 14;
	runaway_rates.timed.float1d(14).identifier.description = "Avalanche onset field (V/m)";
	runaway_rates.timed.float1d(14).value.resize(N_rho);
	
	//! Synchrotron loss time initialisation
	runaway_rates.timed.float1d(15).identifier.id = "synchrotron_loss_time";
	runaway_rates.timed.float1d(15).identifier.flag = 15;
	runaway_rates.timed.float1d(15).identifier.description = "Synchrotron loss time (s)";
	runaway_rates.timed.float1d(15).value.resize(N_rho);
	
	//! Normalised synchrotron loss time initialisation
	runaway_rates.timed.float1d(16).identifier.id = "norm_synchrotron_loss_time";
	runaway_rates.timed.float1d(16).identifier.flag = 16;
	runaway_rates.timed.float1d(16).identifier.description = "Normalised synchrotron loss time";
	runaway_rates.timed.float1d(16).value.resize(N_rho);
	
	//! Toroidicity for Dreicer
	runaway_rates.timed.float1d(17).identifier.id = "toroidicity_dreicer";
	runaway_rates.timed.float1d(17).identifier.flag = 17;
	runaway_rates.timed.float1d(17).identifier.description = "Toroidicity for Dreicer rate";
	runaway_rates.timed.float1d(17).value.resize(N_rho);
		
	//! Toroidicity for Avalanche
	runaway_rates.timed.float1d(18).identifier.id = "toroidicity_avalanche";
	runaway_rates.timed.float1d(18).identifier.flag = 18;
	runaway_rates.timed.float1d(18).identifier.description = "Toroidicity for Avalanche rate";
	runaway_rates.timed.float1d(18).value.resize(N_rho);

	//! Toroidicity for Avalanche
	runaway_rates.timed.float1d(19).identifier.id = "relative_electric_field";
	runaway_rates.timed.float1d(19).identifier.flag = 19;
	runaway_rates.timed.float1d(19).identifier.description = "Relative electric field (by critical field)";
	runaway_rates.timed.float1d(19).value.resize(N_rho);

	return 0;
}

