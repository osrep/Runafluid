#include <cmath>
#include "growth_rate.h"

//! 	\f[	m_\mathrm{e} \cdot c^2 \f]
double me_c2 = ITM_ME * pow(ITM_C, 2);

//! 	\f[	m_\mathrm{e}^2 \cdot c^3 \f]
double me2_c3 = me_c2 * ITM_ME * ITM_C;

//! 	\f[	\frac{m_\mathrm{e}^2 \cdot c^3}{e} \f]
double me2_c3__e = me2_c3 / ITM_QE;

//! 	\f[	\frac{4\pi\epsilon_0^2\cdot m_\mathrm{e}^2 c^3}{e^4} \f]
double pi_4_e02_me2_c3__e4 = ITM_PI * 4.0 * pow(ITM_EPS0, 2) * me2_c3 / pow(ITM_QE, 4);



/*!

Growth rate warning

\details The module outputs an integer value (0 or 1) which indicates, whether the growth rate of runaway electron generation is above the limit value given in Demultiplexer workflow, thus runaway generation is to be expected. If the growth rate exceeds the limit value this warning raises.

\param pro profile
\param limit double


 4.2.3 Functional Requirements
\return REQ-1: If a radius exists where growth rate exceeds the limit, returns 1.	
\return REQ-2: If growth rate is below limit across the whole profile, returns 0.
\return REQ-3: Growth rate is calculated 
*/

int is_growth_rate_over_limit(profile pro, double limit) {

	for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
		if (calculate_growth_rate(it->electron_density, it->electron_temperature,
				it->effective_charge, it->electric_field) > limit)
			return 1;
	}

	return 0;
}

//! Calculation of growth rate
/*!
	\param electron_density \f$ n_\mathrm{e}\f$ electron density in \f$10^{20} \mathrm{~m}^{-3}\f$.
	\param electron_temperature \f$ t_\mathrm{e}\f$ electron temperature in \f$\mathrm{keV}\f$.
	\param effective_charge \f$ Z_\mathrm{eff} \f$ effective charge
	\param electric_field	\f$ E_\parallel \f$ electric field in \f$N/C\f$.
	
*/


double calculate_growth_rate(double electron_density, double electron_temperature,
		double effective_charge, double electric_field) {
	
	//! \a REQ-4: Coulomb logarithm
	/*!
	\f[
		\ln \Lambda = 14.9-0.5 \cdot \log \left(n_\mathrm{e} \cdot 10^{-20}\right) + \log \left(t_\mathrm{e} \cdot 10^{-3}\right) .
	\f]
	*/
	double coulomb_log = 14.9 - 0.5 * log(electron_density * 1e-20)
			+ log(electron_temperature * 1e-3);

	/*! electron temperature
	\f[ T_\mathrm{e} \mathrm{[kJ]} = t_\mathrm{e} \mathrm{[keV]} \cdot e\f]
	*/
	electron_temperature *= ITM_QE;


	//! \a REQ-3: diffusion time
	/*! 
	\f[
		\tau = 4 \pi \epsilon_0^2 \cdot \frac{m_\mathrm{e}^2 \cdot c^3 }{e^4} \cdot \frac{1}{n_\mathrm{e} \ln \Lambda}		
	\f]
	*/
	double tao = pi_4_e02_me2_c3__e4 / (electron_density * coulomb_log);

	//! \a REQ-3: Dreicer field
		/*!
	\f[
		E_D = \frac{m_\mathrm{e}^2 c^3}{e\tau \cdot T_\mathrm{e}}		
	\f]
	*/
	double Ed = me2_c3__e / (tao * electron_temperature);


	//! \return \a REQ-3: growth rate
		/*!
	\f[
		\dot{n}_r^I= \frac{n_\mathrm{e}}{\tau} \cdot \left(\frac{m_\mathrm{e} c^2}{2 T_\mathrm{e}} \right)^{3/2}\cdot
		\left( \frac{E_D}{E_\parallel}\right)^{3 \left(1+Z_\mathrm{eff}\right) / 16} \cdot 
		\exp \left( -\frac{E_D}{4E_\parallel} - \sqrt{\frac{1+Z_\mathrm{eff} \cdot E_D}{E_\parallel}}\right)
		
	\f]
	*/

	return electron_density / tao * pow(me_c2 / (2.0 * electron_temperature), 1.5)
			* pow(Ed / electric_field, 3.0 * (1.0 + effective_charge) / 16.0)
			* exp(-Ed / (4.0 * electric_field) - sqrt((1 + effective_charge) * Ed / electric_field));
}
