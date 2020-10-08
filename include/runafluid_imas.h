#ifndef RUNAFLUID_H_
#define RUNAFLUID_H_

#include "control.h"

void fire(const IdsNs::IDS::core_profiles &core_profiles,
		  const IdsNs::IDS::equilibrium &equilibrium, IdsNs::IDS::distributions &distribution_in,
		  const IdsNs::IDS::distributions &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  IdsNs::IDS::temporary &runaway_rates);
		  
module_struct init_modules_from_runafluid_switch(int runafluid_switch);

#endif /* RUNAFLUID_H_ */
