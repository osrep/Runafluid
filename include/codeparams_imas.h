#ifndef CODEPARAMS_H_
#define CODEPARAMS_H_

#include "control.h"

module_struct read_codeparams(IdsNs::codeparam_t &codeparams);
module_struct simulate_codeparams(int runafluid_switch);

std::string stream_xml_string(std::string xml_string, std::string ref);
std::string split_string(std::string s, std::string ref);

#endif /* CODEPARAMS_H_ */
