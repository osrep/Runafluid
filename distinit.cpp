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
		distribution_out.distri_vec.resize(1);
		distribution_out.distri_vec(0).profiles_1d.state.dens/*.value*/ = 0;
		distribution_out.distri_vec(0).profiles_1d.state.current/*.value*/ = 0;
		
	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during distri_vec resize" << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;

	
		//! internal error in distribution
	
	}
	dist_initialised = 1;
}




	
