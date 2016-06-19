#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>

/*
#include "constants.h"
#include "cpo_utils.h"
#include "runafluid.h"
#include "init.h"*/




void fire(ItmNs::Itm::distribution &distribution_in, ItmNs::Itm::distribution &distribution_out, ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur, int &dist_initialised, double &time_next, ItmNs::Itm::temporary &distribution_temp, ItmNs::Itm::temporaryArray &da) {


	
	try {
		//! number of geometry elements
		int N = coreprof.ne.value.rows();
		
	//std::cerr << "Length of distri_vec: \t"<< N << std::endl << std::endl;
		int N_rho_tor = coreprof.rho_tor.rows();
		
	//std::cerr << "Length of distri_vec: \t"<< N_rho_tor << std::endl << std::endl;
		int N_rho_tor_norm = coreprof.rho_tor_norm.rows();
		
	//std::cerr << "Length of distri_vec: \t"<< N_rho_tor_norm << std::endl << std::endl;
		int N_psi = coreprof.psi.value.rows();
		
	//std::cerr << "Length of distri_vec: \t"<< N_psi << std::endl << std::endl;
		int N_area = coreimpur.area.rows();		
		
	//std::cerr << "Length of distri_vec: \t"<< N_area << std::endl << std::endl;
		int N_volume = coreimpur.volume.rows();
		
	//std::cerr << "Length of distri_vec: \t"<< N_volume << std::endl << std::endl;
	
		int flag = 0;
		std::cerr << "dens3prev: " << distribution_in.distri_vec(0).profiles_1d.state.dens(3) << std::endl;
				
		try {			
			flag = distribution_in.distri_vec(0).source_id(0).type.flag;			
		
		} catch (const std::exception& ex) {
			throw std::invalid_argument("distribution non-readable");
		
		}	
		
		if (dist_initialised & flag==7){
			std::cerr << " -----------  PREVIOUS DISTRIBUTION: OK -----------t" << std::endl;
			
		}		
			
				
		//! New distribution data set
		distribution_out.distri_vec.resize(1);	
	
		//! Initialisation of runaway density
		distribution_out.distri_vec(0).profiles_1d.state.dens.resize(N);
	
	
		//! Initialisation of runaway current
		distribution_out.distri_vec(0).profiles_1d.state.current.resize(N);
	
		for (int i = 0; i < N; ++i){
		
			if (dist_initialised & flag==7){
			//! Filling up runaway density and current from the input distribution
				distribution_out.distri_vec(0).profiles_1d.state.dens(i) = distribution_in.distri_vec(0).profiles_1d.state.dens(i);
				distribution_out.distri_vec(0).profiles_1d.state.current(i) = distribution_in.distri_vec(0).profiles_1d.state.current(i);
				
				if(i==10) {std::cerr << "dens10prev: " << distribution_in.distri_vec(0).profiles_1d.state.dens(10) << std::endl;}
			}else{	
			//! Filling up runaway density and current with zeroes
			
				distribution_out.distri_vec(0).profiles_1d.state.dens(i) = 0;
				distribution_out.distri_vec(0).profiles_1d.state.current(i) = 0;
			}	
		}
		
		//! New distribution source
		distribution_out.distri_vec(0).source_id.resize(1);	
	
	
		//! Filling up distribution source
		distribution_out.distri_vec(0).source_id(0).type.id = "runaway";
		distribution_out.distri_vec(0).source_id(0).type.flag = 7;
		distribution_out.distri_vec(0).source_id(0).type.description = "Source from runaway processes";
	
	
		//! Filling up distribution species
		distribution_out.distri_vec(0).species.type.id = "electron";
		distribution_out.distri_vec(0).species.type.flag = 1;
		distribution_out.distri_vec(0).species.type.description = "Electron";
	
	
		//! Filling up distribution gyro type
		distribution_out.distri_vec(0).gyro_type = 1;
	
		//! Initialisation of geometry data		
		distribution_out.distri_vec(0).profiles_1d.geometry.rho_tor.resize(N);
		distribution_out.distri_vec(0).profiles_1d.geometry.rho_tor_norm.resize(N);
		distribution_out.distri_vec(0).profiles_1d.geometry.psi(N);
	
	
		//! Filling up runaway geometry data from the previous distribution
		if (dist_initialised & flag==7){
		
			for (int i = 0; i < N; ++i){
				if (i < N_rho_tor){
					distribution_out.distri_vec(0).profiles_1d.geometry.rho_tor(i) = distribution_in.distri_vec(0).profiles_1d.geometry.rho_tor(i);
				}
		
				if (i < N_rho_tor_norm){
					distribution_out.distri_vec(0).profiles_1d.geometry.rho_tor_norm(i) = distribution_in.distri_vec(0).profiles_1d.geometry.rho_tor_norm(i);
				}			
			}
		
		//! Filling up runaway geometry data from coreprof
		}else{
			
			for (int i = 0; i < N; ++i){
				if (i < N_rho_tor){
					distribution_out.distri_vec(0).profiles_1d.geometry.rho_tor(i) = coreprof.rho_tor(i);
				}
		
				if (i < N_rho_tor_norm){
					distribution_out.distri_vec(0).profiles_1d.geometry.rho_tor_norm(i) = coreprof.rho_tor_norm(i);
				}

				/*if (i < N_psi){
					distribution_out.distri_vec(0).profiles_1d.geometry.psi(i) = coreprof.psi.value(i);
				}
		
				if (i < N_area){
					distribution_out.distri_vec(0).profiles_1d.geometry.area(i) = coreimpur.area(i);
				}
		
				if (i < N_volume){
					distribution_out.distri_vec(0).profiles_1d.geometry.volume(i) = coreimpur.volume(i);
				}*/
			}
		}
		
		
		
		distribution_temp.non_timed.float1d.resize(1);
		distribution_temp.non_timed.float1d(0).value.resize(N);
		//distribution_tempA.array.resize(1);
		/*distribution_tempA.array.resize(1);
		distribution_tempA.array(0).timed.float1d.resize(1);
		distribution_tempA.array(0).timed.float1d(0).value.resize(N);
	*/
	
	
		for (int i = 0; i < N; ++i){
		
				if (dist_initialised & flag==7){
				//! Filling up runaway density and current from the input distribution
					distribution_temp.non_timed.float1d(0).value(i) = distribution_in.distri_vec(0).profiles_1d.state.dens(i);
					//distribution_tempA.array(0).timed.float1d(0).value(i) = distribution_in.distri_vec(0).profiles_1d.state.dens(i);
				}
			}
	
	
	
	
	//Allocate room for 3 CPO instances
	da.array.resize(1);
	da[0].time = next_time;
	da[0].timed.float1d.resize(1);	
	da[0].timed.float1d(0).value.resize(N);
		
		
		
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during distri_vec resize" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

	
		//! internal error in distribution
	
	}
	
	
	
	
}




	
