#include <cmath>
//#include "products.h"
#include "critical_field.h"
//#include "critical_field.cpp"
#include "dreicer.h"
//#include "init.h"

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
		double effective_charge, double electric_field, int formula_id) {
		
		
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
	double coulomb_log = 14.9 - 0.5 * log(electron_density * 1e-20)
			+ log(electron_temperature * 1e-3);


	//! \a REQ-2: electron collision time
	/*! 
	\f[
		\tau = 4 \pi \epsilon_0^2 \cdot \frac{m_\mathrm{e}^2 \cdot c^3 }{e^4} \cdot \frac{1}{n_\mathrm{e} \ln \Lambda}		
	\f]
	*/
	
	double tao = pi_4_e02_me2_c3__e4 / (electron_density * coulomb_log);

	//! \a REQ-1: Dreicer field
		/*!
	\f[
		E_D = \frac{m_\mathrm{e}^2 c^3}{e\tau \cdot T_\mathrm{e}~\mathrm{[J]}}		
	\f]
	*/

	double Ed = calculate_dreicer_field(electron_density, electron_temperature);
//	double Ed_alt = me2_c3/kB_T/IT
	double Ed__E = Ed/electric_field;
	

	//! \a REQ-7: alpha
	/*!
	\f[	
		\alpha = \frac{E}{E_\mathrm{D}} \cdot \frac{m_\mathrm{e} \cdot c^2}{T_\mathrm{e}~\mathrm{[J]}}
	\f]
	*/	
		
	double alpha = electric_field / calculate_critical_field(electron_density, electron_temperature);
	double alpha_2 = alpha*alpha;	
	
	
	//! \a REQ-6: lambda
	/*!
	\f[	
		\lambda(\alpha)=8\alpha\left( \alpha-\frac{1}{2}-\sqrt{\alpha(\alpha-1}\right)
	\f]
	*/
	
	double lambda = 8*alpha*(alpha-1/2-sqrt(alpha*(alpha-1)));
	//cout << "lambda: " << lambda << "\n";
	
	
	//! \a REQ-5: multiplication factor
	/*!
	\f[	
		\gamma(\alpha,Z) = \sqrt{\frac{(1+Z)\alpha^2}{8(\alpha-1)}}\cdot \left( \frac{\pi}{2}-\sin^{-1}\left(1-\frac{2}{\alpha} \right) \right)
	\f]
	*/
	
	double gamma = sqrt((1+effective_charge) * alpha_2/8/(alpha-1)) * (ITM_PI/2-asin(1-2/alpha));
	//cout << "gamma: " << gamma << "\n";


	//! \a REQ-4: h factor
	/*!	
	\f[	
		h = \frac{1}{16(\alpha - 1)}\cdot \left( \alpha \cdot (Z+1)-Z+7+2\cdot\sqrt{\frac{\alpha}{\alpha -1}} \cdot (1+Z)\cdot(\alpha-2)
		\right)
	\f]
	*/

	double h = 1/(16*(alpha-1)) * (alpha*(effective_charge+1) - \
		effective_charge + 7 + 2*sqrt(alpha/(alpha-1)) * (1+effective_charge)*(alpha-2));
	//cout << "h: " << h << "\n";
	
		
	//! runaway limit -- critical field (65)
	/*!
	\f[
		E_\mathrm{R} = \frac{E_\mathrm{D} T}{m_\mathrm{e} c^2}
	\f]
	*/
			
	
	
	double Cr=1.0;	
	double dgr;
	
	if ((formula_id == 67)|| (formula_id == 66)){	
		//!  \return non-relativistic Dreicer generation rate (67)
		/*!
		\f[
			R_\mathrm{NR} = \frac{1}{\tau} \left( \frac{E}{E_\mathrm{D}} \right) ^{-\frac{3}{16}(Z+1)} \cdot \exp \left( - \frac{E_\mathrm{D}}{4E} - \sqrt{(1+Z) \frac{E_\mathrm{D}}{E} }  \right)
		\f]
		*/	
		
		dgr = Cr/tao * pow(Ed__E,3/16*(effective_charge+1)) * exp(-1/4*Ed__E - sqrt((effective_charge+1)*Ed__E));	
	
	
		if (formula_id == 66){
		
		//!  \return Dreicer generation rate with relativistic correction (66)
		/*!
		\f[
			R_\mathrm{R} = R_\mathrm{NR} \cdot \exp \left( - frac{T_\mahtrm{e}}{m c^2} \left( \frac{1}{8} \left( \frac{E_\mathrm{D}}{E}\right)^2 + \frac{2}{3} \left( \frac{E_\mathrm{D}}{E}\right)^{3/2}  \cdot \sqrt{1+Z} \right) \right) 
		\f]
		*/
		
		
		//std::cerr << "Dreicer field:\t" << Ed <<"\t\trelativistic factor:\t" << -kB_T/me_c2 * Ed__E*Ed__E/8 + 2/3*pow(Ed__E,1.5) *sqrt(1+effective_charge) << std::endl;
		
		dgr = dgr * exp(-kB_T/me_c2 * Ed__E*Ed__E/8 + 2/3*pow(Ed__E,1.5) *sqrt(1+effective_charge));
		
		}
	
	}else{
	
		//! \return Dreicer generation rate (63)
		/*!
		\f[
			R_\mathrm{D} = \frac{1}{\tau} \left(\frac{E_\mathrm{D}}{E} \right)^h(\alpha,Z) \cdot \exp{-\frac{\lambda}{4} \cdot \frac{E_\mathrm{D}}{E} - \sqrt{2  \frac{E_\mathrm{D}}{E}} \gamma(\alpha,Z)}
		
		\f]
		*/	
	
		dgr = Cr/tao * pow(Ed__E,h) * exp(-lambda/4*Ed__E - sqrt(2*Ed__E)*gamma); 
	}	
	
	//! Dreicer rate must be non-negative
	if(isnan(dgr)|| (dgr<0)){
		dgr = 0;
	}

	//! output: Dreicer generation rate	
	return dgr;
	
	
}


