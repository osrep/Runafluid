#ifndef CPO_UTILS_H_
#define CPO_UTILS_H_

#include <UALClasses.h>
#include "constants.h"
#include "control.h"
#include "plasma_structures.h"

double sign(double a);

int binary_search(const Array<double, 1> &array, double search_key);

double interpolate(const Array<double, 1> &x, const Array<double, 1> &y, double xa);

int bool_switch(int switch_number, bool *bools, int N);

int get_digit(int number, int digit);

int whereRunaway(ItmNs::Itm::distribution &distribution);

plasma_profile cpo_to_profile(const ItmNs::Itm::coreprof &coreprof);	

plasma_profile cpo_to_profile(const ItmNs::Itm::coreprof &coreprof, const ItmNs::Itm::coreimpur &coreimpur,
		const ItmNs::Itm::equilibrium &equilibrium, const ItmNs::Itm::distribution &distribution);	

#endif /* CPO_UTILS_H_ */
