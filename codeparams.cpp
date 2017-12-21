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

using namespace std;

int set_switch_from_codeparams(ItmNs::codeparam_t &codeparams){
	int runafluid_switch = 0;
	DecITM::DecodeITMpar params(codeparams.parameters);
	std::string parameters;
	parameters = params.get();
	std::string str_dreicer_formula = stream_xml_string(parameters,"dreicer_formula");
	std::string str_dreicer_toroidicity =stream_xml_string(parameters,"dreicer_toroidicity");
	std::string str_avalanche_formula =stream_xml_string(parameters,"avalanche_formula");
	std::string str_avalanche_toroidicity =stream_xml_string(parameters,"avalanche_toroidicity");

	if(!str_dreicer_toroidicity.compare("1") && !str_avalanche_toroidicity.compare("1")){
		runafluid_switch += 1000;	
	}else if(!str_dreicer_toroidicity.compare("1")){
		runafluid_switch += 2000;	
	}else if(!str_avalanche_toroidicity.compare("1")){
		runafluid_switch += 3000;	
	}

	if(!str_avalanche_formula.compare("rosenbluth_putvinski")){
		runafluid_switch += 300;	
	}else if(!str_avalanche_formula.compare("rosenbluth_putvinski_with_threshold")){
		runafluid_switch += 100;	
	}

	if(!str_dreicer_formula.compare("hc_formula_63")){
		runafluid_switch += 10;	
	}else if(!str_dreicer_formula.compare("hc_formula_66")){
		runafluid_switch += 20;	
	}else if(!str_dreicer_formula.compare("hc_formula_67")){
		runafluid_switch += 30;	
	}
	return runafluid_switch;
}

modules read_codeparams(ItmNs::codeparam_t &codeparams){
	modules m;
	DecITM::DecodeITMpar params(codeparams.parameters);
	std::string parameters;
	parameters = params.get();
	std::string str_dreicer_formula = stream_xml_string(parameters,"dreicer_formula");
	std::string str_dreicer_toroidicity = stream_xml_string(parameters,"dreicer_toroidicity");
	std::string str_avalanche_formula = stream_xml_string(parameters,"avalanche_formula");
	std::string str_avalanche_toroidicity = stream_xml_string(parameters,"avalanche_toroidicity");
	std::string str_output_path = stream_xml_string(parameters,"output_path");

	if(str_dreicer_toroidicity.compare("1")){
		m.dreicer_toroidicity = true;
	}else{
		m.dreicer_toroidicity = false;
	}

	if(str_avalanche_toroidicity.compare("1")){
		m.avalanche_toroidicity = true;
	}else{
		m.avalanche_toroidicity = false;
	}

	m.dreicer_formula = str_dreicer_formula;
	m.avalanche_formula = str_avalanche_formula;
	m.output_path = str_output_path;
	/*std::cout << "Dreicer formula\t" << str_dreicer_formula  << std::endl;
	std::cout << "Avalanche formula\t" << str_avalanche_formula  << std::endl;
	std::cout << "Output path\t" << str_output_path  << std::endl;
	if (m.output_path.empty()){		
		std::cout << "NO HDF5\t" << std::endl;
	}*/

	return m;
}
	

modules simulate_codeparams(int runafluid_switch){
	
	modules m;
	return m;
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
		//std::cout << "--> "<< token << std::endl;

		for(int i=0; ((pos2 = token.find(delimiter2)) != std::string::npos);i++){
			token2 = token.substr(0, pos2);
			//std::cout <<"     "<<i<<"   "<< token2 << std::endl;
			token.erase(0, pos2 + delimiter2.length());
			//std::cout <<"     "<<i<<"  "<<token<<" >>>   "<< token << std::endl;
			if (!token2.compare(ref)){
				std::cout <<"  [Runaway Fluid] \t Code Parameter for  "<<token2<<" is "<< token << std::endl;
				xml_value = token;
			}
		}

		s.erase(0, pos + delimiter.length());
	
	}
	//std::cout << s << std::endl;
	return xml_value;

}



std::string stream_xml_string(std::string xml_string, std::string ref){

	/*std::cout <<  "---- From the Actor ---" << std::endl;
	std::cout << xml_string << std::flush;*/
	
	return split_string(xml_string,ref);
}
