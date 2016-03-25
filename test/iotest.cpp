#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>


/*! 

runaway distribution reader

*/
void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution, double &out1, double &out2, char &out3) {

		try {
			out1=1;//coreprof.profiles1d.eparallel.value.rows();
			out2=2;//coreprof.profiles1d.eparallel.value(1);
			out3='3';
		} catch (const std::exception& ex) {
			std::cerr << "ERROR An error occurred during firing actor Runaway Indicator." << std::endl;
			std::cerr << "ERROR : " << ex.what() << std::endl;
		}

}
