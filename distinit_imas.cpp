#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>
#include "distinit.h"

/*
#include "constants.h"
#include "cpo_utils.h"
#include "runafluid.h"
#include "init.h"*/




void distinit(IdsNs::IDS::distribution &distribution, IdsNs::IDS::core_profiles &core_profiles, int timeindex) {
	
	int N=0;
	int N_rho_tor=0;
	int N_rho_tor_norm=0;
	int N_psi=0;
	 
	/*  
	//! number of coreprof geometry elements
	try {		
		N = coreprof.ne.value.rows();		
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
	
	try {			
		N_psi = coreprof.psi.value.rows();		
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during coreprof elements" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
			
	//! New distribution data set	
	try {	
		distribution.distri_vec.resize(1);	
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during distri_vec allocation" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	try {		
		//! Initialisation of runaway density
		distribution.distri_vec(0).profiles_1d.state.dens.resize(N);

		//! Initialisation of runaway current
		distribution.distri_vec(0).profiles_1d.state.current.resize(N);
		
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during density vector allocation" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	//! Filling up distribution density and current
	try {	
		for (int i = 0; i < N; ++i){		
			distribution.distri_vec(0).profiles_1d.state.dens(i) = 0;
			distribution.distri_vec(0).profiles_1d.state.current(i) = 0;
		}
	} catch (const std::exception& ex) {
		std::cerr << "[Runaway Fluid] ERROR: An error occurred during density vector filling" << std::endl;
		std::cerr << "[Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	//! Filling up metadata
	try {	
		//! New distribution source
		distribution.distri_vec(0).source_id.resize(1);	
	
		//! Filling up distribution source
		distribution.distri_vec(0).source_id(0).type.id = "runaway";
		distribution.distri_vec(0).source_id(0).type.flag = 7;
		distribution.distri_vec(0).source_id(0).type.description = "Source from runaway processes";

		//! Filling up distribution species
		distribution.distri_vec(0).species.type.id = "electron";
		distribution.distri_vec(0).species.type.flag = 1;
		distribution.distri_vec(0).species.type.description = "Electron";

		//! Filling up distribution gyro type
		distribution.distri_vec(0).gyro_type = 1;	
			
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred metadata filling" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	
	//! Initialisation of geometry data	
	try {	
		distribution.distri_vec(0).profiles_1d.geometry.rho_tor.resize(N_rho_tor);
		distribution.distri_vec(0).profiles_1d.geometry.rho_tor_norm.resize(N_rho_tor_norm);
		distribution.distri_vec(0).profiles_1d.geometry.psi(N_psi);
		
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during geometry vectors allocation" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}
	
	
	//! Filling up coreprof geometry data	
	try {			
		for (int i = 0; i < N; ++i){
			if (i < N_rho_tor){
				distribution.distri_vec(0).profiles_1d.geometry.rho_tor(i) = coreprof.rho_tor(i);
			}
	
			if (i < N_rho_tor_norm){
				distribution.distri_vec(0).profiles_1d.geometry.rho_tor_norm(i) = coreprof.rho_tor_norm(i);
			}
			
			
		//	if (i < N_psi){
		//		distribution.distri_vec(0).profiles_1d.geometry.psi(i) = coreprof.psi.value(i);
		//	}
			
		}
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during coreprof geometry vectors filling" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}

	
	//! Filling up codeparam
	
	try {
		distribution.codeparam.codename = "Runaway Fluid (runafluid)";
		distribution.codeparam.codeversion = "1.6.2";
		distribution.codeparam.output_diag = "Runaway Fluid was initialised successfully";
		distribution.codeparam.output_flag = 0;
	} catch (const std::exception& ex) {
		std::cerr << "  [Runaway Fluid] ERROR: An error occurred during filling codeparam" << std::endl;
		std::cerr << "  [Runaway Fluid] ERROR: " << ex.what() << std::endl;
	}*/
	
}




	
