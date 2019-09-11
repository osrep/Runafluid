#include <cmath>

#include <stdexcept>
#include <iostream>

#include "dreicer.h"
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

double dreicer_generation_rate(double electron_density, double electron_temperature,
		double effective_charge, double electric_field, double rho_tor_norm, int formula_id) {
		
		
	//! electron temperature: electronvolt to joule
	/*!
	\f[
		T_\mathrm{e}~\mathrm{[J]} = e \cdot t_\mathrm{e}~\mathrm{[eV]} (= k_\mathrm{B} \cdot T_\mathrm{e}~\mathrm{[K]})
	\f]
	*/	
		
	double kB_T = electron_temperature* ITM_EV;

	//! \a REQ-3: Coulomb logarithm
	/*!
	\f[
		\ln \Lambda = 14.9-0.5 \cdot \log \left(n_\mathrm{e} \cdot 10^{-20}\right) + \log \left(t_\mathrm{e} \cdot 10^{-3}\right) .
	\f]
	*/
	double coulomb_log = calculate_coulomb_log(electron_density, electron_temperature);


	//! \a REQ-2: electron collision time
	/*! 
	\f[
		\tau = 4 \pi \epsilon_0^2 \cdot \frac{m_\mathrm{e}^2 \cdot c^3 }{e^4} \cdot \frac{1}{n_\mathrm{e} \ln \Lambda}		
	\f]
	*/
	
	double thermal_electron_collision_time = calculate_thermal_electron_collision_time(electron_density, electron_temperature);

	//! \a REQ-1: Dreicer field
		/*!
	\f[
		E_D = \frac{m_\mathrm{e}^2 c^3}{e\tau \cdot T_\mathrm{e}~\mathrm{[J]}}		
	\f]
	*/

	double Ed = calculate_dreicer_field(electron_density, electron_temperature);
	double Ed__E = Ed/electric_field;
	

	//! \a REQ-7: relative electric field
	/*!
	\f[	
		\alpha = \frac{E}{E_\mathrm{D}} \cdot \frac{m_\mathrm{e} \cdot c^2}{T_\mathrm{e}~\mathrm{[J]}}
	\f]
	*/	
		
	double alpha = calculate_alpha(electric_field,electron_density,electron_temperature);
		
	//! \a REQ-6: lambda: C&H (64)
	/*!
	\f[	
		\lambda(\alpha)=8\alpha\left( \alpha-\frac{1}{2}-\sqrt{\alpha(\alpha-1}\right)
	\f]
	*/
	
	double lambda = calculate_lambda(alpha);
	
	//! \a REQ-5: multiplication factor: C&H (64)
	/*!
	\f[	
		\gamma(\alpha,Z) = \sqrt{\frac{(1+Z)\alpha^2}{8(\alpha-1)}}\cdot \left( \frac{\pi}{2}-\sin^{-1}\left(1-\frac{2}{\alpha} \right) \right)
	\f]
	*/
	
	double gamma = calculate_gamma(effective_charge,alpha);

	//! \a REQ-4: h factor: C&H (62)
	/*!	
	\f[	
		h = \frac{1}{16(\alpha - 1)}\cdot \left( \alpha \cdot (Z+1)-Z+7+2\cdot\sqrt{\frac{\alpha}{\alpha -1}} \cdot (1+Z)\cdot(\alpha-2)
		\right)
	\f]
	*/

	double h = calculate_h(alpha,effective_charge);

	//! runaway limit -- critical field: C&H  (65)
	/*!
	\f[
		E_\mathrm{R} = \frac{E_\mathrm{D} T}{m_\mathrm{e} c^2}
	\f]
	*/

	double Cr=1.0;	
	double dgr;
	
  
	if ( (modules.dreicer_formula == "hc_formula_67") || (modules.dreicer_formula == "hc_formula_66") ){
			dgr = Cr/thermal_electron_collision_time * pow(Ed__E,3.0/16.0*(effective_charge+1.0)) * exp(-1.0/4.0*Ed__E - sqrt((effective_charge+1.0)*Ed__E));
	
		if (modules.dreicer_formula == "hc_formula_66"){

			dgr = dgr * exp(-kB_T/me_c2 * (Ed__E*Ed__E/8.0 + 2.0/3.0*pow(Ed__E,1.5) *sqrt(1.0+effective_charge)));
		}
	
	}else{
	
		//! \return Dreicer generation rate: C&H  (63)
		/*!
		\f[
			S_\mathrm{D} = n_\mathrm{e} \cdot \frac{1}{\tau} \left(\frac{E_\mathrm{D}}{E} \right)^{h(\alpha,Z)} \cdot \exp \left(-\frac{\lambda(\alpha)}{4} \cdot \frac{E_\mathrm{D}}{E} - \sqrt{2  \frac{E_\mathrm{D}}{E}} \cdot \gamma(\alpha,Z) \right)
		\f]
		*/	
	// 
		
		dgr = Cr/thermal_electron_collision_time * pow(Ed__E,h) * exp(-lambda/4.0*Ed__E - sqrt(2.0*Ed__E)*gamma); 
	}	
		
	//! Dreicer rate must be non-negative
	if(isnan(dgr)|| (dgr<0)){
		dgr = 0;
		//std::cerr << "ERROR: Dreicer generation rate is invalid. Set to zero." << std::endl;
		
	}

	//! output: Dreicer generation rate	
	return dgr;
}



double dreicer_generation_rate(double electron_density, double electron_temperature,
		double effective_charge, double electric_field, double rho_tor_norm, modules m) {		
		
	double kB_T = electron_temperature* ITM_EV;
	double coulomb_log = calculate_coulomb_log(electron_density, electron_temperature);
	double thermal_electron_collision_time = calculate_thermal_electron_collision_time(electron_density, electron_temperature);
	double Ed = calculate_dreicer_field(electron_density, electron_temperature);
	double Ed__E = Ed/electric_field;	
	double alpha = calculate_alpha(electric_field,electron_density,electron_temperature);
	double lambda = calculate_lambda(alpha);
	double gamma = calculate_gamma(effective_charge,alpha);
	double h = calculate_h(alpha,effective_charge);

	double Cr = 1.0;	
	double dgr;
	
	if ( (m.dreicer_formula.compare("hc_formula_67")) || (m.dreicer_formula.compare("hc_formula_66")) ){			
			dgr = Cr/thermal_electron_collision_time * pow(Ed__E,3.0/16.0*(effective_charge+1.0)) * exp(-1.0/4.0*Ed__E - sqrt((effective_charge+1.0)*Ed__E));
	
		if (m.dreicer_formula.compare("hc_formula_66")){
			dgr = dgr * exp(-kB_T/me_c2 * (Ed__E*Ed__E/8.0 + 2.0/3.0*pow(Ed__E,1.5) *sqrt(1.0+effective_charge)));		
		}
	
	} else dgr = Cr/thermal_electron_collision_time * pow(Ed__E,h) * exp(-lambda/4.0*Ed__E - sqrt(2.0*Ed__E)*gamma); 
	
		
	//! Dreicer rate must be non-negative
	if ( isnan(dgr) || (dgr<0) ) dgr = 0;

	//! output: Dreicer generation rate	
	return dgr;
}


/*Inner calculations of dreicer generation rate*/

double calculate_alpha(double electric_field, double electron_density, double electron_temperature){
	return electric_field / calculate_critical_field(electron_density, electron_temperature);
}

double calculate_lambda(double alpha){
	return 8.0*alpha*(alpha-1.0/2.0-sqrt(alpha*(alpha-1.0)));
}

double calculate_gamma(double effective_charge, double alpha){
	return sqrt((1.0+effective_charge) * alpha*alpha/8.0/(alpha-1.0)) * (ITM_PI/2.0-asin(1.0-2.0/alpha));
}
double calculate_h(double alpha, double effective_charge){
	return (1.0/(16.0*(alpha-1.0)) * (alpha*(effective_charge+1.0) -
			effective_charge + 7.0 + 2.0*sqrt(alpha/(alpha-1.0)) * (1.0+effective_charge)*(alpha-2.0)));
}

double calculate_toroidicity_dreicer(double inv_asp_ratio, double rho_tor_norm){
    double inv_asp_ratio_coord = inv_asp_ratio*rho_tor_norm;
	return (1.0 - 1.2*sqrt((2.0*inv_asp_ratio_coord)/(1.0+inv_asp_ratio_coord)));
}

