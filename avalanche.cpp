#include <cmath>

#include <stdexcept>
#include <iostream>

#include "avalanche.h"
#include "products.h"
#include "critical_field.h"

using namespace std;

/*! Critical field warning

\details The module outputs an integer value (0 or 1) which indicates, whether electric field is above the critical level, thus runaway generation is possible. When the electric field exceeds the critical level this warning raises, a value of 1 is outputted. However it does not mean that runaway electrons are present, the warning only signs the possibility.

\param pro profile

 4.1.3 Functional Requirements
\return \a REQ-1: If a radius exists where electric field is above critical, returns 1.	
\return \a REQ-2: If electric field is below critical across the whole profile, returns 0.
\return \a REQ-3: Critical electric field 

*/

double avalanche_generation_rate(double electron_density, double electron_temperature,
		double effective_charge, double electric_field, double magnetic_field, int modulevar_avalanche) {
				
	//! \a REQ-1: Coulomb logarithm
	/*!
	\f[
		\ln \Lambda = 14.9-0.5 \cdot \log \left(n_\mathrm{e} \cdot 10^{-20}\right) + \log \left(t_\mathrm{e} \cdot 10^{-3}\right) .
	\f]
	*/
	double coulomb_log = calculate_coulomb_log(electron_density, electron_temperature);
		

	//! \a REQ-2: Critical electric field
	
	double Ec = calculate_critical_field(electron_density, electron_temperature); 


	//! \a REQ-3: electron collision time
	/*! 
	\f[
		\tau = 4 \pi \epsilon_0^2 \cdot \frac{m_\mathrm{e}^2 \cdot c^3 }{e^4} \cdot \frac{1}{n_\mathrm{e} \ln \Lambda}		
	\f]
	*/
		
	double runaway_collision_time = calculate_runaway_collision_time(electron_density, electron_temperature);	
	double synchrotron_loss_time = calculate_synchrotron_loss_time(magnetic_field);
	double norm_synchrotron_loss_time = synchrotron_loss_time/runaway_collision_time;

	//! \return Avalanche generation rate	
	//\Delta n_r \approx \frac{n_\mathrm{r}}{2 \tau \ln \Lambda} \left(\frac{E}{E_\mathrm{c}} -1 \right)   ~~~~~~~ \mathrm{(if~}	E \ge E_\mathrm{a}	\mathrm{)}	
	/*!
	\f[
		R_\mathrm{a} \approx \frac{1}{2 \tau \ln \Lambda} \left(\frac{E}{E_\mathrm{c}} -1 \right)   ~~~~~~~ \mathrm{(if~}	E \ge E_\mathrm{a}	\mathrm{)}		
		
	\f]
	*/		
	
	double agr, Ea;
	
	
	if (modulevar_avalanche == 1) {modulevar_avalanche = 2;}
	
	if (modulevar_avalanche == 2 || modulevar_avalanche == 3){
	
	
	
		agr = (electric_field/Ec - 1) / (2*runaway_collision_time*coulomb_log);
	
	
		/*! 
		\f[
			\Delta n_r = 0 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  \mathrm{(if~}	E < E_\mathrm{a}	\mathrm{)}
		\f]
		*/
	

		//! threshold field: Ea
		
		if (modulevar_avalanche == 2){
			
			/*! 
		\f[
			E_\mathrm{a} \approx E_0 = 1 + \frac{ \frac {1+Z}{sqrt(\tau_\mathrm{rad}}}{(\frac{1}{8} + \frac{(Z+1)^2}{\tau_\mathrm{rad}})^{1/6}}
		\f]
		*/
		
			Ea = 1 + (1+effective_charge) / sqrt(norm_synchrotron_loss_time) / pow( 1.0/8.0 + (1+effective_charge) * (1+effective_charge) / norm_synchrotron_loss_time , 1.0/6.0);
			Ea *= Ec;
		}else{
			Ea = 0;	
		}
	
		if (electric_field < Ea){
			agr = 0;
		}	
	
		//! Avalanche rate must be non-negative
		if(isnan(agr)|| (agr<0)){
			agr = 0;
		}
	}
	
	return agr;
	
	
}




double calculate_toroidicity_avalanche(double inv_asp_ratio, double electric_field, double electron_density, double electron_temperature){

	double critical_field = calculate_critical_field(electron_density, electron_temperature);
	double rel_electric_field = electric_field/critical_field;
	
	return pow(1.0-inv_asp_ratio,2)/sqrt(ITM_PI*sqrt(inv_asp_ratio*rel_electric_field);

}



double calculate_flow_Ap(double p, double E, double Z){
	return 2*E/(Z+1)*p*p/sqrt(p*p+1);
}

double calculate_flow_costheta(double p, double E, double Z){
	double Ap = calculate_flow_Ap(p,E,Z);
	return 1/(tanh(Ap))- 1/Ap;
}

double calculate_flow_velocity(double electron_density, double electron_temperature, double effective_charge, double electric_field, double magnetic_field){

	double Ec = calculate_critical_field(electron_density, electron_temperature);
	double E = electric_field/Ec;
	double Z = effective_charge;
	double p = 0; // temporary definition

	double costheta = calculate_flow_costheta(p,E,Z);
	return 0;
}

