#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "constants.h"
#include "dreicer.h"
#include "dreicer.cpp"

using namespace std;



int main(void){

	double electron_density = 1e20;//e20;//m^-3
	double electron_temperature = 1218;//eV
	double effective_charge  = 	1.0;
	double electric_field = 6.46;//3.991;// V/m
	
	double rate=-999999999;
	rate = dreicer_generation_rate(electron_density, electron_temperature,
		effective_charge, electric_field);
		
		
		
	cout << "Dreicer generation: " << rate << "\n";	
	
	double rate2 = rate/electron_density;
	
	cout << "Dreicer generation rate: " << rate2 << "\n";	
	
 
	return 0;

}
