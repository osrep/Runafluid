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




void fire(ItmNs::Itm::distribution &distribution_in, ItmNs::Itm::distribution &distribution_out, ItmNs::Itm::coreprof &coreprof, int &dist_initialised) {


//std::cerr << "Number of elements:"<< std::endl;
//distri_vec[*]/source_id/type/flag == 7

	//! length of distri_vec
	
	
	/*int N_dist = dist_in.distri_vec.rows();
	
	std::cerr << "Length of distri_vec: \t"<< N_dist << std::endl << std::endl;
	
	for (int i=0;i<N_dist;i++){
		std::cerr << "\t DISTRI_VEC["<< i << "]: \t" << dist_in.distri_vec(i).source_id(0).type.flag << dist_in.distri_vec(i).source_id(0).type.id << std::endl;
	}
	*/
	
	try {
		//! number of geometry elements
		int N = distribution_out.distri_vec(0).profiles_1d.state.dens.rows();

		
		;/*.value = 0;*/
		distribution_out.distri_vec(0).profiles_1d.state.current.resize(N);/*.value = 0;*/
		
		std::cerr << "resized dv" << std::endl;
		
		
		distribution_out.distri_vec(0).source_id.resize(1);	
		std::cerr << "resized sid" << std::endl;
		
		distribution_out.distri_vec(0).source_id(0).type.id = "runaway";
		distribution_out.distri_vec(0).source_id(0).type.flag = 7;
		distribution_out.distri_vec(0).source_id(0).type.description = "Source from runaway processes";
		
		std::cerr << "source_id0" << std::endl;
		
		
	//	std::cerr << "resized spec" << std::endl;
		
		distribution_out.distri_vec(0).species.type.id = "electron";
		distribution_out.distri_vec(0).species.type.flag = 1;
		distribution_out.distri_vec(0).species.type.description = "Electron";
		
		std::cerr << "species" << std::endl;
		distribution_out.distri_vec(0).gyro_type = 1;
		
		
		
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during distri_vec resize" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

	
		//! internal error in distribution
	
	}
	dist_initialised = 1;
	
	
	/*try {
		std::cerr << "TIME:\t"<< getShot()	 << std::endl;
	} catch (const std::exception& ex) {
		std::cerr << "ERROR sECOND TRY	" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

	
		//! internal error in distribution
	
	}*/
}




	