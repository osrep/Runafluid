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


void fire(ItmNs::Itm::distribution &dist_in, ItmNs::Itm::distribution &dist_out, int &dist_initialised) {


//std::cerr << "Number of elements:"<< std::endl;
//distri_vec[*]/source_id/type/flag == 7

	//! length of distri_vec
	int N_dist = dist_in.distri_vec.rows();
	
	std::cerr << "Length of distri_vec: \t"<< N_dist << std::endl << std::endl;
	
	for (int i=0;i<N_dist;i++){
		std::cerr << "\t"<< i << ": \t" << dist_in.distri_vec(i).source_id(0).type.flag/*.value()*/ << std::endl;
	}
	
	dist_initialised = 1;
}




	
