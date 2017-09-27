#ifndef RUNAFLUID_H_
#define RUNAFLUID_H_

void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		  ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution_in,
		  ItmNs::Itm::distribution &distribution_out, double &timestep, int &runafluid_switch,
		  double &critical_fraction, int &runaway_warning, int &not_suitable_warning,
		  int &critical_fraction_warning, ItmNs::Itm::temporary &runaway_rates, ItmNs::codeparam_t&);
		
int init_rates(ItmNs::Itm::temporary & runaway_rates, int N_rates, int N_rho);

set_switch_from_codeparams(ItmNs::codeparam_t &codeparams);

std::string stream_xml_string(std::string xml_string, std::string ref);

std::string split_string(std::string s, std::string ref);

#endif /* RUNAFLUID_H_ */
