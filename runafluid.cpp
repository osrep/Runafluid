#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

#include "constants.h"
#include "cpo_utils.h"
//#include "critical_field.h"

#include "dreicer.h"
#include "avalanche_rate.h"
// double growth_rate_limit = 1e12;


/*! \mainpage

Purpose

This document specifies software requirements of the Runaway Indicator (runin) software module. The module is developed and deployed in the European Transport Solver (ETS) framework maintained by the Code Development for Integrated Modelling Project (ITM) of the EUROfusion consortium (http://portal.efda-itm.eu).


Product Scope

The Runaway Indicator (runin) project supplies a simulator module assembled as a Kepler workflow actor, which is capable of indicating whether runaway electron generation is to be expected during tokamak operation. This functionality is highly valuable in ITM simulations, since present equilibrium and transport calculations neglect the generation of runaway electrons. The runin module can determine whether runaways are generated thus validate the results of equilibrium and transport modules in this manner.

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

*/
void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		ItmNs::Itm::equilibriumArray &equilibrium, int steps) {



	/*dreicer_rate = 0.0;
	avalanche_rate = 0.0*/;
	int dreicer_error = 1;
	/*int*/ steps = 0;
	
	
	try {
	//! critical field: \sa is_field_critical
	
		steps = step_counter(cpo_to_profile(coreprof, coreimpur, equilibrium));
		
	/*	double
		
		dreicer_error = init_dreicer(
				cpo_to_profile(coreprof, coreimpur, equilibrium)
				);
*/
	

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runaway Indicator." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in critical_field
		steps = ITM_ILLEGAL_INT;
		
	}
}

/*!
main function

arrays by time*/

/*
void fire(ItmNs::Itm::coreprofArray &coreprof, ItmNs::Itm::coreimpurArray &coreimpur,
		ItmNs::Itm::equilibriumArray &equilibrium, double &ea, double &dreicer_rate, double &avalanche_rate) {

	dreicer_rate = 0.0;
	avalanche_rate = 0.0;
	
	int dreicer_error = 0;
	int avalanche_error = 0;

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
				
				
				//! Initialisation of Dreicer module
				dreicer_error = init_dreicer(
						cpo_to_profile(coreprof[slice], coreimpur[slice], equilibrium[slice]));
				
				


		}

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runaway Fluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

		//! internal error in critical_field
		critical_field_warning = ITM_ILLEGAL_INT;
		
		//! internal error in growth_rate
		growth_rate_warning = ITM_ILLEGAL_INT;
	}
}*/
	
