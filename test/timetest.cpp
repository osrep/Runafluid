#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>


/*! 

timetest

*/
void fire(ItmNs::Itm::distribution &distribution, ItmNs::Itm::distributionArray &distarray, double &output) {

	int slices = distarray.array.rows();
	
	for (int slice = 0; slice < slices; slice++) {
		std::cerr << "slice " << slice << "\t " << distarray[slice].time;
		if (equal(distribution.time, distarray[slice].time , 0.01)) {
		
			std::cerr << "\t <-------------";
		}else{
			std::cerr << "\t XXX";		
		}
		
		std::cerr << std::endl;
	}

}
