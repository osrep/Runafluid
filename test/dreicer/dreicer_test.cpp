#include <stdexcept>
#include <iostream>
#include <fstream>
#include <math.h> 
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
		std::ofstream datafile, datafilog, dataf0, dataf1, dataf2, dataf0l;
  		datafile.open ("dreicer.dat");
  		datafilog.open ("dreicer_log.dat");
  		
  		
  		
		datafile << "#" << "electric field" << "\t" << "electron temperature" << "\t"  << "dreicer generation"  << "\n";
		
		datafilog << "#" << "electric field (log10)" << "\t" << "electron temperature (log10)" << "\t"  << "dreicer generation (log10)"  << "\n";
  		
  		
  		dataf0.open ("dreicer0.dat");	
  		dataf0l.open ("dreicer0l.dat");
  		
  		dataf1.open ("dreicer1.dat");
  		dataf2.open ("dreicer2.dat");
  		
		for (int i=0;i<50;i++){
			electron_temperature = pow(10,i/10.0);			
			dataf2 << electron_temperature << "\t";
			
  			for (int j=0;j<50;j++){
  			
  				electric_field= pow(10,j/10.0-2.0);
			//! Calculate Dreicer generation rate
				rate_dreicer = dreicer_generation_rate(electron_density, electron_temperature,
				effective_charge, electric_field);
				
				if (isnan(rate_dreicer)){
					rate_dreicer =1e99;
				}
				
				if (j==0){
					dataf0 << rate_dreicer;
					dataf0l << log10(rate_dreicer);
				}else{	
					dataf0 << "\t" << rate_dreicer ;
					dataf0l << "\t" << log10(rate_dreicer) ;
				}
				
				
				
				datafile << electric_field << "\t" << electron_temperature << "\t"  << rate_dreicer  << "\n";
				datafilog << log10(electric_field) << "\t" << log10(electron_temperature) << "\t"  << log10(rate_dreicer)  << "\n";
				
			}
			dataf0 << "\n";
			dataf0l << "\n";
		}
		//datafile << "test\n";
		//! close data writing
		datafile.close();
		datafilog.close();
		dataf0.close();
		dataf0l.close();
		dataf1.close();
		dataf2.close();
			
				

	} catch (const std::exception& ex) {
		std::cerr << "ERROR An error occurred during firing actor Runafluid." << std::endl;
		std::cerr << "ERROR : " << ex.what() << std::endl;
		
		//! internal error in distribution
		
	}
	return 0;
}
