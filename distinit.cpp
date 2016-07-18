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




void distinit(ItmNs::Itm::distribution &distribution, ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur) {
	
	int N=0;
	int N_rho_tor=0;
	int N_rho_tor_norm=0;
	int N_psi=0;
	int N_area=0;
	int N_volume=0;
	 
	  
	//! number of coreprof geometry elements
	try {		
		N = coreprof.ne.value.rows();		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during coreprof elements" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}	
	
	try {		
		N_rho_tor = coreprof.rho_tor.rows();
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during coreprof elements" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	
	try {			
		N_rho_tor_norm = coreprof.rho_tor_norm.rows();		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during coreprof elements" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	try {			
		N_psi = coreprof.psi.value.rows();		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during coreprof elements" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	/*
	//! number of coreimpur geometry elements
	try {
		N_area = coreimpur.area.rows();			
		N_volume = coreimpur.volume.rows();
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during coreimpur elements" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}	
	*/
		
	//! New distribution data set	
	try {	
		distribution.distri_vec.resize(1);	
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during distri_vec allocation" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	try {		
		//! Initialisation of runaway density
		distribution.distri_vec(0).profiles_1d.state.dens.resize(N);

		//! Initialisation of runaway current
		distribution.distri_vec(0).profiles_1d.state.current.resize(N);
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during density vector allocation" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	//! Filling up distribution density and current
	try {	
		for (int i = 0; i < N; ++i){		
			distribution.distri_vec(0).profiles_1d.state.dens(i) = 0;
			distribution.distri_vec(0).profiles_1d.state.current(i) = 0;
		}
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during density vector filling" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
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
		std::cerr << "ERROR An error occurred metadata filling" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	
	//! Initialisation of geometry data	
	try {	
		distribution.distri_vec(0).profiles_1d.geometry.rho_tor.resize(N_rho_tor);
		distribution.distri_vec(0).profiles_1d.geometry.rho_tor_norm.resize(N_rho_tor_norm);
		distribution.distri_vec(0).profiles_1d.geometry.psi(N_psi);
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during geometry vectors allocation" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
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
		std::cerr << "ERROR An error occurred during coreprof geometry vectors filling" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
	//! Filling up coreimpur geometry data	
	try {
	
		if(N_area==0){
			std::cerr << "WARNING: coreimpur.area is empty" << std::endl;
		}
		for (int i = 0; i < N; ++i){
	
		//	if (i < N_area){
		//		distribution.distri_vec(0).profiles_1d.geometry.area(i) = coreimpur.area(i);
		//	}
	
		//	if (i < N_volume){
		//		distribution.distri_vec(0).profiles_1d.geometry.volume(i) = coreimpur.volume(i);
		//	}
		}
			
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during coreprof geometry vectors filling" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
	}
	
}




	
