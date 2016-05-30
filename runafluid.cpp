#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "constants.h"
#include "cpo_utils.h"
#include "runafluid.h"
#include "init.h"

// double growth_rate_limit = 1e12;


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
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution, ItmNs::Itm::distribution &distribution_prev, double &timestep, int &runafluid_switch/*, ItmNs::Itm::temporary &tempData*/) {


	std::cerr << "Number of elements:" << distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.rows() << "|" << coreprof.ne.value.rows() << std::endl;

	//ItmNs::Itm::distribution &distribution2 = distribution.getSlice();

//	distribution2.distri_vec(0).put(distribution.distri_vec(0).get());
/*
	try {
		distribution.distri_vec.resize(1);
		std::cerr << "distri_vec resized" << std::endl;
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during distri_vec resize" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

	
		//! internal error in distribution
	
	}*/
		
		
	/*try {
		distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.resize(coreprof.ne.value.rows());
		std::cerr << "distribution array resized" << std::endl;
		
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during distribution array resize" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	
		//! internal error in distribution
	
	}


	std::cerr << "Number of elements:" << distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.rows() << "|" << coreprof.ne.value.rows() << std::endl;

*/
	try {
			
		double rundensity = 0.0;
		
		
		//! set distribution CPO with default data
//		distributionInit(distribution_prev, distribution, coreprof);
		
		//! reading profile from CPO inputs (cpo_utils.h)
		profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);
		
		
		
		
		
		//! stepping iterator in profile	
		int rho = 0;	
		
		

		
		for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
			rundensity = runafluid_control(it->electron_density, it->runaway_density, it->electron_temperature, it->effective_charge, it->electric_field, timestep, runafluid_switch);
			
			if(rho==0){
			std::cerr << "DISTSOURCE_IDENTIFIER : " << DISTSOURCE_IDENTIFIER << std::endl;
			}
			
			
			std::cerr << "OUT : " << rundensity << std::endl;

			if (rho<distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens.rows()){
				if(rundensity > it->electron_density){
			   		distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = it->electron_density;
			   		std::cerr << rho<<"\tMAX\t" << rundensity << std::endl;
		   		}else if (rundensity < 0  || isnan(rundensity)){
			   		distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = 0;					   		
			   		std::cerr << rho<<"\tZERO\t" << rundensity << std::endl;   		
		   		}else{
			   		distribution.distri_vec(DISTSOURCE_IDENTIFIER).profiles_1d.state.dens(rho) = rundensity;
			   		std::cerr << rho<<"\tVALUE\t" << rundensity << std::endl;
		   		}
		   	}
		    rho++;
		
		}		
		
				
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}
	

}





/*!
main function

arrays by time
void fire(ItmNs::Itm::coreprofArray &coreprof, ItmNs::Itm::coreimpurArray &coreimpur,
		ItmNs::Itm::equilibriumArray &equilibrium,	int &critical_field_warning, int &step_counter) {

	critical_field_warning = 0;

	try {
		int slices = coreprof.array.rows();
		if (coreimpur.array.rows() != slices)
			throw std::invalid_argument(
					"Number of cpo slices is different in coreprof and coreimpur.");
		if (equilibrium.array.rows() != slices)
			throw std::invalid_argument(
					"Number of cpo slices is different in coreprof and equilibrium.");

		int slice = 0;
		for (slice = 0; slice < slices; slice++) {

			if (!equal(coreprof[slice].time, coreimpur[slice].time, 0.01)
					|| !equal(coreprof[slice].time, equilibrium[slice].time, 0.01))
				throw std::invalid_argument("Time value differs in cpo slices of the same index.");

			//! critical field: \sa is_field_critical
			if (critical_field_warning == 0) {
				critical_field_warning = is_field_critical(
						cpo_to_profile(coreprof[slice], coreimpur[slice], equilibrium[slice]));
					
				step_counter = stepCounter(
						cpo_to_profile(coreprof[slice], coreimpur[slice], equilibrium[slice]));
						

					
			}

	
		}

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runaway Indicator." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

		//! internal error in critical_field
		critical_field_warning = ITM_ILLEGAL_INT;
		step_counter = ITM_ILLEGAL_INT;
		
	}
}*/
	
