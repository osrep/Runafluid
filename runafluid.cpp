#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "constants.h"
#include "cpo_utils.h"
#include "distinit.h"
#include "control.h"

/*! \mainpage

Purpose

This document specifies software requirements of the Runaway Fluid (runafluid) software module. The module is developed and deployed in the European Transport Solver (ETS) framework maintained by the Code Development for Integrated Modelling Project (ITM) of the EUROfusion consortium (http://portal.efda-itm.eu).

Intended Audience and Reading Suggestions

This Software Requirements Specification (SRS) document is intended for ITM/ETS/H&CD workflow developers, and developers of runafluid module. The document describes the module runafluid, laying out functional and non-functional requirements. Purpose, overview of the module, interfaces, and CPO objects used, constraints, assumptions and dependencies, functional requirements are contained in this documentation.


Product Scope

The Runaway Fluid (Runafluid) project supplies a simulator module assembled as a Kepler workflow actor, which is capable of indicating whether runaway electron generation is to be expected during tokamak operation. This functionality is highly valuable in ITM simulations, since present equilibrium and transport calculations neglect the generation of runaway electrons. The Runafluid module can determine whether runaways are generated thus validate the results of equilibrium and transport modules in this manner.

\subpage references

*/


/*! \page references References

Basic description is provided at http://portal.efda-itm.eu/twiki/bin/view/Main/HCD-ElectronRun-awayPhysics?sso_from=bin/view/Main/HCD-ElectronRun-awayPhysics.

User manual is maintained at http://portal.efda-itm.eu/twiki/bin/view/Main/HCD-codes-runin-usermanual.

Runaway Indicator is maintained under the ITM-TF Collaborative Software Development Environment using Gforge. The project documentation is accessible via http://gforge.efda-itm.eu/gf/project/runin/. Source code is stored in the SVN repository https://gforge.efda-itm.eu/svn/runin.

Analytical formula used to determine the critical electric field is based on the work of J.W. Connor and R.J. Hastie [1]. The method of calculating Dreicer runaway generation growth rate stems from the article of H. Dreicer [2].

[1] A. Stahl, E. Hirvijoki, J. Decker, O. Embréus, and T. Fülöp. Effective Critical Electric Field for Runaway-Electron Generation. Physical Review Letters 114(11), 115002 (2015)

[2] H. Smith, P. Helander, L.-G. Eriksson, D. Anderson, M. Lisak, and F. Andersson, Runaway electrons and the evolution of the plasma current in tokamak disruptions,  Physics of Plasmas 13, 102502 (2006)

*/




/*! 

main function

fix time label

runafluid_switch:

ABCD

 A
   0: Electric field from input
   1: Electric field set to 1000  
   
 B
   0: Calculate density
   1: Previous density
   
 
 C  
   0: Dreicer  OFF
   1: Dreicer  ON

 D
   0: Avalanche  OFF
   1: Avalanche  ON



*/


void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution_in, ItmNs::Itm::distribution &distribution_out, double &timestep, int &runafluid_switch, double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning, ItmNs::Itm::temporary &runaway_rates) {


	//! start: runafluid
	std::cerr << " START: runafluid" << std::endl;
	
	//! get time
	double time = coreprof.time;
	
	//! output initialisation
	runaway_warning = 0;
	not_suitable_warning = 0;
	critical_fraction_warning = 0;
	
	//! zero limit
	double zero_threshold = 1e-20;
	
	//! empty distribution initialiser (integrated distinit)
	distinit(distribution_out, coreprof, coreimpur);	
	
	//! Number of elements in runaway electron distribution
	int N_rho = coreprof.ne.value.rows();
		
	//! runaway density and current initialisation	
	double rundensity = 0.0;
	double runcurrent = 0.0;
	
	//! electron current initialisation
	double ecurrent = 0.0;		
	
	//! reading profile from CPO inputs (cpo_utils.h)
	profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution_in); // testing until previous distribution validating
			
	//! Number of rate calculations (Dreicer, Avalanche etc.)
	//int N_rates = 6;
	int N_rates = 10;
	double rate_values[N_rates];	
		
	//! stepping iterator in profile	
	int rho = 0;	
	
	//! output flag
	int output_flag = 0;
	
	//! runaway_rates for generation rates
	//! Dreicer generation rate initialisation
	runaway_rates.timed.float1d.resize(N_rates);
	runaway_rates.timed.float1d(0).identifier.id = "dreicer";
	runaway_rates.timed.float1d(0).identifier.flag = 0;
	runaway_rates.timed.float1d(0).identifier.description = "Dreicer generation rate";
	runaway_rates.timed.float1d(0).value.resize(N_rho);		
	
	runaway_rates.timed.float1d(2).identifier.id = "dreicer63";
	runaway_rates.timed.float1d(2).identifier.flag = 2;
	runaway_rates.timed.float1d(2).identifier.description = "Dreicer generation rate by Connor et al. (63)";
	runaway_rates.timed.float1d(2).value.resize(N_rho);				
	
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
	
	//! Avalanche generation rate initialisation
	runaway_rates.timed.float1d(1).identifier.id = "avalanche";
	runaway_rates.timed.float1d(1).identifier.flag = 1;
	runaway_rates.timed.float1d(1).identifier.description = "Avalanche generation rate";
	runaway_rates.timed.float1d(1).value.resize(N_rho);
	
	//! Dreicer field
	runaway_rates.timed.float1d(6).identifier.id = "dreicerf01";
	runaway_rates.timed.float1d(6).identifier.flag = 6;
	runaway_rates.timed.float1d(6).identifier.description = "Dreicer field (formula 1)";
	runaway_rates.timed.float1d(6).value.resize(N_rho);
	
	runaway_rates.timed.float1d(7).identifier.id = "dreicerf02";
	runaway_rates.timed.float1d(7).identifier.flag = 7;
	runaway_rates.timed.float1d(7).identifier.description = "Dreicer field (formula 2)";
	runaway_rates.timed.float1d(7).value.resize(N_rho);
	
	//! critical field
	runaway_rates.timed.float1d(8).identifier.id = "criticalf";
	runaway_rates.timed.float1d(8).identifier.flag = 8;
	runaway_rates.timed.float1d(8).identifier.description = "Critical field";
	runaway_rates.timed.float1d(8).value.resize(N_rho);
	
	//! Coulomb logarithm
	runaway_rates.timed.float1d(9).identifier.id = "coulomblog";
	runaway_rates.timed.float1d(9).identifier.flag = 9;
	runaway_rates.timed.float1d(9).identifier.description = "Coulomb logarithm";
	runaway_rates.timed.float1d(9).value.resize(N_rho);
	

	//! Distribution source index for output
	int distsource_out_index = 0;
	
	for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			
		//! Length of the runaway distribution is correct
		if (rho<distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows()){
				
			//! calculating runaway density
			rundensity = runafluid_control(it->electron_density, it->runaway_density, it->electron_temperature, it->effective_charge, abs(it->electric_field), timestep, runafluid_switch, rate_values);
					  
		   	//! CPO output -- runaway warning
	   		if (rundensity > zero_threshold){
				runaway_warning = 1;				
				//std::cerr << "[Runaway Fluid] Warning: Runaway electrons detected (" << rho << ")" << std::endl;
			}else{
				rundensity = 0; // no runaway
			}
			
			//!  critical fraction warning
	   		if (rundensity > critical_fraction/100.0*it->electron_density){
				critical_fraction_warning = 1;
				//std::cerr << "[Runaway Fluid] Warning: Runaway density is higher than the critical fraction: " << critical_fraction << "%% (" << rho << ")" << std::endl;
			}
		   	
		   	//! runaway density n_R
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
				//std::cerr << "[Runaway Fluid] Warning: Runaway current is higher than electron current (" << rho << ")" << std::endl;
		   	}	   	
		   	
		   	
		   	//! runaway rates (Dreicer, Avalanche etc.)
		   	for(int rates_i=0;rates_i<N_rates;++rates_i){
		   		runaway_rates.timed.float1d(rates_i).value(rho) = rate_values[rates_i];
			}	
	   		
	   	}else{		   	
			std::cerr << "[Runaway Fluid] ERROR: The length of runaway distribution array is incorrect(" << rho << "/" << distribution_out.distri_vec(distsource_out_index).profiles_1d.state.dens.rows() << ")" << std::endl;
	   	}   	   	
	   
	    rho++;
	
	}
	
		   	
   	//! error messages to dump
   	if (runaway_warning == 1){				
		std::cerr << "[Runaway Fluid] Warning: Runaway electrons detected at " << time << " s" << std::endl;
	}		
	
	if (critical_fraction_warning == 1){				
		std::cerr << "[Runaway Fluid] Warning: Runaway density is higher than the range of validity (critical fraction: " << critical_fraction << "%)  at " << time << " s" << std::endl;
		output_flag -= 1;
	}	
	
	if (not_suitable_warning == 1){				
		std::cerr << "[Runaway Fluid] Warning: Runaway current is higher than electron current at " << time << " s" << std::endl;
		output_flag -= 10;
	}	
	
	//! output flag to distribution CPO
	try {
		distribution_out.codeparam.output_flag = output_flag;
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during filling output_flag in codeparam" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	distribution_out.time = distribution_in.time+timestep;
	
	//! end: runafluid
	std::cerr << " END: runafluid" << std::endl;

}
