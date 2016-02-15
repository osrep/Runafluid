#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "constants.h"
#include "avalanche_rate.h"
#include "avalanche_rate.cpp"

using namespace std;



int main(void){

	double electron_density = 1e20;//e20;//m^-3
	double electron_temperature = 1.2;//1000.0;//eV
	double effective_charge  = 	2.0;
	double electric_field = 0.08;// V/m
	double dt = 1e-10; //s
	
	double rate=-999999999;
	rate = avalanche_generation_rate(electron_density, electron_temperature,
		effective_charge, electric_field,dt);
		
		
		
	cout << "Avalanche generation: " << rate << "\n";	
	
	double rate2 = rate/electron_density;
	
	cout << "Avalanche generation rate: " << rate2 << "\n";	
	
 
	return 0;

}
