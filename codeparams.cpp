#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <UALClasses.h>
#include <libxml/xmlreader.h>
#include "DecodeITMpar.h"		
#include <unistd.h>
#include "H5Cpp.h"
#include "codeparams.h"
#include "control.h"
#include <stdlib.h>

using namespace std;

module_struct read_codeparams(ItmNs::codeparam_t &codeparams){
	module_struct modules;
	DecITM::DecodeITMpar params(codeparams.parameters);
	std::string parameters;
	parameters = params.get();
	std::string str_dreicer_formula = stream_xml_string(parameters,"dreicer_formula");
	std::string str_dreicer_toroidicity = stream_xml_string(parameters,"dreicer_toroidicity");
	std::string str_avalanche_formula = stream_xml_string(parameters,"avalanche_formula");
	std::string str_avalanche_toroidicity = stream_xml_string(parameters,"avalanche_toroidicity");
	std::string str_hdf5_output = stream_xml_string(parameters,"hdf5_output");
	std::string str_warning_percentage_limit = stream_xml_string(parameters, "warning_percentage_limit");
	std::string str_rho_edge_calculation_limit = stream_xml_string(parameters, "rho_edge_calculation_limit");

	double dbl_warning_percentage_limit = atof(str_warning_percentage_limit.c_str());
	double dbl_rho_edge_calculation_limit = atof(str_rho_edge_calculation_limit.c_str());

	if(str_dreicer_toroidicity == "true"){
		modules.dreicer_toroidicity = true;
	}else{
		modules.dreicer_toroidicity = false;
	}

	if(str_avalanche_toroidicity == "true"){
		modules.avalanche_toroidicity = true;
	}else{
		modules.avalanche_toroidicity = false;
	}

	if(str_hdf5_output == "true"){
		modules.hdf5_output = true;
	}else{
		modules.hdf5_output = false;
	}

	modules.dreicer_formula = str_dreicer_formula;
	modules.avalanche_formula = str_avalanche_formula;
	modules.warning_percentage_limit = dbl_warning_percentage_limit;
	modules.rho_edge_calculation_limit = dbl_rho_edge_calculation_limit;

	return modules;
}
	

module_struct simulate_codeparams(int runafluid_switch){
	
	module_struct modules;
	return modules;
}

std::string split_string(std::string s, std::string ref){

	std::string delimiter = "<";
	std::string delimiter2 = ">";
	std::string xml_value = ""; 
	size_t pos = 0;
	size_t pos2 = 0;
	std::string token, token2;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);

		for(int i=0; ((pos2 = token.find(delimiter2)) != std::string::npos);i++){
			token2 = token.substr(0, pos2);
			token.erase(0, pos2 + delimiter2.length());
			if (!token2.compare(ref)){
				std::cout <<"  [Runaway Fluid] \t Code Parameter for  "<<token2<<" is "<< token << std::endl;
				xml_value = token;
			}
		}

		s.erase(0, pos + delimiter.length());
	
	}

	return xml_value;

}



std::string stream_xml_string(std::string xml_string, std::string ref){

	return split_string(xml_string,ref);
}
