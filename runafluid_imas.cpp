#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "runafluid_imas.h"
#include "constants.h"
#include "ids_utils.h"
#include "distinit.h"
#include "control.h"

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

[1] A. Stahl, E. Hirvijoki, J. Decker, O. Embréus, and T. Fülöp. Effective Critical Electric Field for Runaway-Electron Generation.
Physical Review Letters 114(11), 115002 (2015)

[2] H. Smith, P. Helander, L.-G. Eriksson, D. Anderson, M. Lisak, and F. Andersson, Runaway electrons and the evolution
of the plasma current in tokamak disruptions,  Physics of Plasmas 13, 102502 (2006)

*/


/*! 

main function

fix time label

runafluid_switch:

ABCD

 A
   0:
   1: 
   
 B
   0: 
   1: 

 C  
   0: 
   1: 

 D
   0: 
   1: 

*/

void fire(IdsNs::IDS::core_profiles &core_profiles,
		  IdsNs::IDS::equilibrium &equilibrium, IdsNs::IDS::distributions &distribution_in,
		  IdsNs::IDS::distributions &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  IdsNs::IDS::temporary &runaway_rates) {

	//! start: runafluid
	std::cerr << " START: runaway_fluid" << std::endl;
	
	//! parse codeparam
	module_struct modules = init_modules_from_runafluid_switch(runafluid_switch);
	
	//! get time
	//double time = coreprof.time;
	int timeindex = 0;
	
	//! output initialisation
	runaway_warning = 0;
	not_suitable_warning = 0;
	critical_fraction_warning = 0;
	
	//! zero limit
	double zero_threshold = 1e-20;
	
	//! maximal normalised minor radius
	double rho_max = 0.95;
	
	//! empty distribution initialiser (integrated distinit)
	distinit(distribution_out, core_profiles, 0);	
	
	//! Number of elements in runaway electron distribution
	int N_rho = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();
		
	//! runaway density and current initialisation	
	double rundensity = 0.0;
	double runcurrent = 0.0;
	
	//! electron current initialisation
	double ecurrent = 0.0;		
	
	//! reading profile from CPO inputs (cpo_utils.h)
	profile pro = ids_to_profile(core_profiles, equilibrium, distribution_in, timeindex); // testing until previous distribution validating
		
	//! stepping iterator in profile	
	int i = 0;	
	
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
	double inv_asp_ratio = equilibrium.time_slice(timeindex).boundary.minor_radius / equilibrium.vacuum_toroidal_field.r0;
	
	//! Runaway fluid switch message	
	runafluid_switch_message(modules);

	//! Distribution source index for output
	int distsource_out_index = 0;
	
	for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
		//! Length of the runaway distribution is correct
		if (i<distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).density.rows()){
				
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
	   		distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).density(i) = rundensity;
		  
		   	//! runaway current
		   	/*!
		   	
		   	j = n_R q_e c \mathrm{sign}(E)
		   	
		   	*/
		   			   	
		   	runcurrent = rundensity * ITM_QE * ITM_C * sign(it->electric_field);		   	
		   	distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).current_tor(i) = runcurrent;
		   	
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
			std::cerr << "  [Runaway Fluid] ERROR: The length of runaway distribution array is incorrect(" << i << "/"
					  << distribution_out.distribution(distsource_out_index).profiles_1d(timeindex).density.rows() << ")" << std::endl;
	   	}   	   	
	   
	    i++;
	
	}

   	//! error messages to dump
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

	//! output flag to distribution CPO
	try {
		distribution_out.code.output_flag(timeindex) = output_flag;
		/*
		if (output_flag == 1){			
			distribution_out.code.output_diag = "Runaway Fluid was running successfully and runaway electrons indicated";
		}
		else if (output_flag == 2){
			distribution_out.code.output_diag = "Runaway Fluid was running successfully but runaway current is higher than electron current";
		}
		else if (output_flag == 3){
			distribution_out.code.output_diag = "Runaway Fluid was running successfully but results out of the range of validity";
		}*/
		
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during filling output_flag in codeparam" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	distribution_out.time(timeindex) = distribution_in.time(timeindex)+timestep;
	
	//! end: runafluid
	std::cerr << " END: runaway_fluid" << std::endl;
}

module_struct init_modules_from_runafluid_switch(int runafluid_switch){
	
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
