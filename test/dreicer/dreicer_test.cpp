#include <stdexcept>
#include <iostream>
#include <fstream>
#include "../../critical_field.cpp"
#include "../../dreicer.cpp"

using namespace std;

/*!
Runafluid Dreicer tester

*/

int main(){
	
	double rate_dreicer = 0.0;
	
	double electron_density = 1e21;//5e19;//1e20;//e20;//m^-3
	double electron_temperature = 24000;//52000;//1218;//eV
	double effective_charge  = 	1.0;
	double electric_field = 0.2;//0.1925;//0.0962	;//0.10;//6.46;//3.991;// V/m
	
	
		
	try {
	
		//! Initialise data file
		std::ofstream datafile, dataf1, dataf2;
  		datafile.open ("dreicer.dat");
  		
  		
  		dataf1.open ("dreicer1.dat");
  		dataf2.open ("dreicer2.dat");
  		
		for (int i=0;i<50;i++){
			electron_temperature = pow(10,i/10);			
			dataf2 << electron_temperature << "\t";
			
  			for (int j=0;j<50;j++){
  			
  				electric_field= pow(10,j/10-2);
			//! Calculate Dreicer generation rate
				rate_dreicer = dreicer_generation_rate(electron_density, electron_temperature,
				effective_charge, electric_field);
		
				if (j==0){
					datafile << rate_dreicer;
				}else{	
					datafile << "\t" << rate_dreicer ;
				}
			}
			datafile << "\n";
		}
		//datafile << "test\n";
		//! close data writing
		datafile.close();
			
				

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}
	return 0;
}
