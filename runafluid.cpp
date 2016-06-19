#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "constants.h"
#include "cpo_utils.h"
#include "runafluid.h"
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
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution, ItmNs::Itm::temporary &distribution_temp, double &timestep, int &runafluid_switch, int &runafluid_warnings, ItmNs::Itm::temporary &runaway_rates/*, ItmNs::Itm::temporaryArray &da*/) {

	//! Number of elements in runaway electron distribution
	int N_rho = distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.rows();
	
	try {
			std::cerr << "RUNAFLUID TEST 001" << std::endl;
		double rundensity = 0.0;
		
		//!!! temp
		/*double rundensity63 = 0.0;
		double rundensity66 = 0.0;
		double rundensity67 = 0.0;*/
		
		//! Number of rate calculations (Dreicer, Avalanche etc.)
		int N_rates = 6;
		double rate_values[N_rates];
		
			std::cerr << "RUNAFLUID TEST 002" << std::endl;
		
		//! reading profile from CPO inputs (cpo_utils.h)
		profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution_temp/*, distribution_prev*/);
		
			
		//! stepping iterator in profile	
		int rho = 0;	
		
			std::cerr << "RUNAFLUID TEST 003" << std::endl;
		//! warning initialiser
		/*runafluid_warnings = 0;
		int warning_n = 2;
		bool warning_bools[warning_n];		
		int_switch(runafluid_warnings,warning_bools,warning_n);*/
		
			std::cerr << "RUNAFLUID TEST 004" << std::endl;
		
		//! Length of previous distribution
		int Ntemp= distribution_temp.non_timed.float1d(0).value.rows();
		
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

		
			std::cerr << "RUNAFLUID TEST 005" << std::endl;
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
		
		
			std::cerr << "RUNAFLUID TEST 006" << std::endl;
			//! Length of the runaway distribution is correct
			if (rho<distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.rows()){
			
						
				//! calculating runaway density
				rundensity = runafluid_control(it->electron_density, it->runaway_density, it->electron_temperature, abs(it->effective_charge), it->electric_field, timestep, runafluid_switch, rate_values);
			   	
			   	if(rho<Ntemp){
				   	rate_values[5]=distribution_temp.non_timed.float1d(0).value(rho);
			   	}
			   	
			std::cerr << "RUNAFLUID TEST 007" << std::endl;
			   	//! CPO output
			   	distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = rundensity;
			   	
			   	for(int rates_i=0;rates_i<N_rates;++rates_i){
			   		runaway_rates.timed.float1d(rates_i).value(rho) = rate_values[rates_i];
				}
				
			std::cerr << "RUNAFLUID TEST 008" << std::endl;
				
				//! runaway warning
				/*if(rundensity > 1e-100){
			   		warning_bools[0] = 1;
		   		}
				
				//! 1\%{} n_e warning
				if(rundensity > 0.01*it->electron_density){
			   		warning_bools[1] = 1;
		   		}*/
				
				
			std::cerr << "RUNAFLUID TEST 009" << std::endl;
				if(rundensity > it->electron_density){
			   		distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = it->electron_density;
		   		}else if (rundensity < 0  || isnan(rundensity)){
			   		distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = 0;				
		   		}else{
		   		}
		   	}else{		   	
				std::cerr << "ERROR The length of runaway distribution array is incorrect" << std::endl;
		   	}
		    rho++;
		
		}
		
			std::cerr << "RUNAFLUID TEST 010" << std::endl;
		/*runafluid_warnings = bool_switch(warning_bools,warning_n);	*/				

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}
	

}
