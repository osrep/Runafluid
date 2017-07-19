#ifndef IDS_UTILS_H_
#define IDS_UTILS_H_

#include <UALClasses.h>
#include "constants.h"
#include "control.h"

bool equal(double a, double b, double tolerance);

int binary_search(const Array<double, 1> &array, double search_key);

double interpolate(const Array<double, 1> &x, const Array<double, 1> &y, double xa);

int get_digit(int number, int digit);

int whereRunaway(IdsNs::IDS::distributions &distributions);
		
profile ids_to_profile(const IdsNs::IDS::core_profiles &core_profiles, const IdsNs::IDS::equilibrium &equilibrium, const IdsNs::IDS::distributions &distributions, int timeindex);

#endif /* IDS_UTILS_H_ */
