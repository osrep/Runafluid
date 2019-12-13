#ifndef RUNAFLUID_H_
#define RUNAFLUID_H_

void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		  ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution_in,
		  ItmNs::Itm::distribution &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning,
		  int &critical_fraction_warning,  int &shot_number,
		  int &run_number, ItmNs::codeparam_t&);

std::string to_string( int x );

#endif /* RUNAFLUID_H_ */
