#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>
#include "distinit.h"




void distinit(IdsNs::IDS::distributions &distributions, IdsNs::IDS::core_profiles &core_profiles, int timeindex) {
	
	int N=0;
	int N_rho_tor=0;
	int N_rho_tor_norm=0;
	int N_psi=0;
	 
	  
	//! number of coreprof geometry elements
	try {		
		N = core_profiles.profiles_1d(timeindex).electrons.density.rows();		
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during coreprof elements" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}	
	
	try {		
		N_rho_tor = core_profiles.profiles_1d(timeindex).grid.rho_tor.rows();
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during coreprof elements" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	
	try {			
		N_rho_tor_norm = core_profiles.profiles_1d(timeindex).grid.rho_tor_norm.rows();;		
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during coreprof elements" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
			
	//! New distribution data set	
	try {	
		distributions.distribution.resize(1);	
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during distri_vec allocation" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	
	//! New distribution data set	
	try {	
		distributions.distribution(0).profiles_1d.resize(1);	
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during profiles_1d timeslice allocation" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	try {		
		//! Initialisation of runaway density
		distributions.distribution(0).profiles_1d(0).density.resize(N);

		//! Initialisation of runaway current
		distributions.distribution(0).profiles_1d(0).current_tor.resize(N);
		
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during density vector allocation" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	//! Filling up distribution density and current
	try {	
		for (int i = 0; i < N; ++i){		
			distributions.distribution(0).profiles_1d(0).density(i) = 0;
			distributions.distribution(0).profiles_1d(0).current_tor(i) = 0;
		}
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during density vector filling" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	//! Filling up metadata
	try {	
		//! New distribution source
		distributions.distribution(0).process.resize(1);	
	
		//! Filling up distribution source
		distributions.distribution(0).process(0).type.name = "runaway";
		distributions.distribution(0).process(0).type.index = 7;
		distributions.distribution(0).process(0).type.description = "Source from runaway processes";

		//! Filling up distribution species
		distributions.distribution(0).species.type.name = "electron";
		distributions.distribution(0).species.type.index = 1;
		distributions.distribution(0).species.type.description = "Electron";

		//! Filling up distribution gyro type
		distributions.distribution(0).gyro_type = 1;	
			
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred metadata filling" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	
	//! Initialisation of geometry data
	
	try {	
		distributions.distribution(0).profiles_1d(0).grid.rho_tor.resize(N_rho_tor);
		distributions.distribution(0).profiles_1d(0).grid.rho_tor_norm.resize(N_rho_tor_norm);
		
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during geometry vectors allocation" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	
	//! Filling up distribution geometry data	
	try {			
		for (int i = 0; i < N; ++i){
			if (i < N_rho_tor){
        		distributions.distribution(0).profiles_1d(0).grid.rho_tor(i) = core_profiles.profiles_1d(0).grid.rho_tor(i);
			}
	
			if (i < N_rho_tor_norm){
        		distributions.distribution(0).profiles_1d(0).grid.rho_tor_norm(i) = core_profiles.profiles_1d(0).grid.rho_tor_norm(i);
			}
			
						
		}
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during coreprof geometry vectors filling" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}

	
	//! Filling up codeparam
	
	try {
		distributions.code.name = "Runaway Fluid (runafluid)";
		distributions.code.output_flag = 0;
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during filling codeparam" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
}




	
