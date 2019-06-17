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
		  ItmNs::Itm::distribution &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  ItmNs::Itm::temporary &runaway_rates, ItmNs::codeparam_t &codeparams) {

	//! start: runafluid
	std::cout << " START: runaway_fluid" << std::endl;
	
	//old switch
	if(runafluid_switch!=0){
		std::cout << "  [Runaway Fluid] \tWarning: A new Runaway_Fluid actor released where runafluid_switch removed. Please read documentation about how to use Code Parameters!"
		<< std::endl << "\t\t\tMore info:\thttp://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runafluid-usermanual"<< std::endl;
	}

	//! parse codeparam
	module_struct modules = read_codeparams(codeparams);

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
	int rho = 0;	
	
	//! output flag
	int output_flag = 0;
	
	//! Number of rate calculations (Dreicer, Avalanche etc.)
	double rate_values[N_RATES];
	int pro_size = pro.size();
	blitz::Array<double,1> dreicer_prof(pro_size);
	blitz::Array<double,1> avalanche_prof(pro_size);
	blitz::Array<double,1> electric_field_prof(pro_size);
	blitz::Array<double,1> critical_field_prof(pro_size);
	
	//! inverse aspect ratio \eps = a/R
	double inv_asp_ratio = equilibrium.eqgeometry.a_minor / coreprof.toroid_field.r0;
	
	//! Runaway fluid switch message	
	runafluid_switch_message(modules);
	bool hdf5_switch = true;

	//! Distribution source index for output
	int distsource_out_index = 0;
	
	for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
		//! Length of the runaway distribution is correct
		if (rho<distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows()){
				
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
	   		distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens(rho) = rundensity;
		  
		   	//! runaway current
		   	/*!
		   	
		   	j = n_R q_e c \mathrm{sign}(E)
		   	
		   	*/
		   			   	
		   	runcurrent = rundensity * ITM_QE * ITM_C * sign(it->electric_field);		   	
		   	distribution_out.distri_vec(distsource_out_index).profiles_1d.state.current(rho) = runcurrent;
		   	
		   	//! not suitable warning: j_R > j_e	
		   	ecurrent = it->electron_density * ITM_QE * ITM_C * sign(it->electric_field);
		   	if (runcurrent/ecurrent >= 1){		   	
				not_suitable_warning = 1;	 	
		   	}
		   	
			dreicer_prof[rho]        = rate_values[RATEID_DREICER];
			avalanche_prof[rho]      = rate_values[RATEID_AVALANCHE];
			electric_field_prof[rho] = rate_values[RATEID_ELECTRIC_FIELD];
			critical_field_prof[rho] = rate_values[RATEID_CRITICAL_FIELD];
	   		
	   	}else{		   	
			std::cerr << "  [Runaway Fluid] \tERROR: The length of runaway distribution array is incorrect(" << rho << "/"
					  << distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows() << ")" << std::endl;
	   	}   	   	
	   
	    rho++;
	
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
	if (!modules.output_path.empty()){
			H5std_string hdf5_file_name(modules.output_path);

			int dataset_name_length = 14; 
			string dataset_name_list[dataset_name_length] = {
				"time","rho_tor","rho_tor_eq",
				"density", "temperature", "eparallel","b0", "zeff",
				"runaway_density","runaway_current","dreicer_rate","avalanche_rate",
				"electric_field_vs_critical_field", "critical_field"};
			int cols = rho;//sizeof dataext / sizeof(double);
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
				write_data_to_hdf5(hdf5_file_name, "dreicer_rate", dreicer_prof);
				write_data_to_hdf5(hdf5_file_name, "avalanche_rate", avalanche_prof);
				write_data_to_hdf5(hdf5_file_name, "electric_field_vs_critical_field", electric_field_prof);
				write_data_to_hdf5(hdf5_file_name, "critical_field", critical_field_prof);
				
			}else{
				cout << "  [Runaway Fluid] \tHDF5 init was not successful." << endl;
			}
	}

	//! end: runafluid
	std::cout << " END: runaway_fluid" << std::endl;
}

