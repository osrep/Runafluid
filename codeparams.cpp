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
