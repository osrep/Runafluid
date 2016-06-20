#ifndef CPO_UTILS_H_
#define CPO_UTILS_H_

#include <UALClasses.h>
#include "constants.h"
#include "control.h"

bool equal(double a, double b, double tolerance);

int binary_search(const Array<double, 1> &array, double search_key);

double interpolate(const Array<double, 1> &x, const Array<double, 1> &y, double xa);

int int_switch(int switch_number, bool *bools, int N);

int bool_switch(bool *bools, int N);

profile read_coreprof(const ItmNs::Itm::coreprof &coreprof) ;	

profile read_coreprof_equilibrium(const ItmNs::Itm::coreprof &coreprof, const ItmNs::Itm::equilibrium &equilibrium) ;	

/*profile cpo_to_profile(const ItmNs::Itm::coreprof &coreprof, const ItmNs::Itm::coreimpur &coreimpur,
		const ItmNs::Itm::equilibrium &equilibrium, const ItmNs::Itm::temporary &distribution);*/

profile cpo_to_profile(const ItmNs::Itm::coreprof &coreprof, const ItmNs::Itm::coreimpur &coreimpur,
		const ItmNs::Itm::equilibrium &equilibrium, const ItmNs::Itm::distribution &distribution);
		

#endif /* CPO_UTILS_H_ */
