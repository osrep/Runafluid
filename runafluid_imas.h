#ifndef RUNAFLUID_H_
#define RUNAFLUID_H_

void fire(const IdsNs::IDS::core_profiles &core_profiles,
		  const IdsNs::IDS::equilibrium &equilibrium, IdsNs::IDS::distributions &distribution_in,
		  const IdsNs::IDS::distributions &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning,
		  IdsNs::IDS::temporary &runaway_rates);
		
int init_rates(IdsNs::IDS::temporary &runaway_rates, int N_rates, int N_rho);

#endif /* RUNAFLUID_H_ */
