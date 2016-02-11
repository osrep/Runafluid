#include <cmath>
#include "avalanche_rate.h"
using namespace std;


//! \f$ e^3 \f$
double e3 = pow(ITM_QE, 3);

//! \f$ 4\pi\epsilon_0^2 m_\mathrm{e} c^2 \f$
double pi_e02_me_4_c2 = ITM_PI * pow(ITM_EPS0, 2) * ITM_ME * 4.0 * pow(ITM_C, 2);


//! 	\f[	m_\mathrm{e} \cdot c^2 \f]
double me_c2 = ITM_ME * pow(ITM_C, 2);

//! 	\f[	m_\mathrm{e}^2 \cdot c^3 \f]
double me2_c3 = me_c2 * ITM_ME * ITM_C;

//! 	\f[	\frac{m_\mathrm{e}^2 \cdot c^3}{e} \f]
double me2_c3__e = me2_c3 / ITM_QE;

//! 	\f[	\frac{4\pi\epsilon_0^2\cdot m_\mathrm{e}^2 c^3}{e^4} \f]
double pi_4_e02_me2_c3__e4 = ITM_PI * 4.0 * pow(ITM_EPS0, 2) * me2_c3 / pow(ITM_QE, 4);



/*! Critical field warning

\details The module outputs an integer value (0 or 1) which indicates, whether electric field is above the critical level, thus runaway generation is possible. When the electric field exceeds the critical level this warning raises, a value of 1 is outputted. However it does not mean that runaway electrons are present, the warning only signs the possibility.

\param pro profile


 4.1.3 Functional Requirements
\return \a REQ-1: If a radius exists where electric field is above critical, returns 1.	
\return \a REQ-2: If electric field is below critical across the whole profile, returns 0.
\return \a REQ-3: Critical electric field 

*/
int is_field_critical(profile pro) {

	for (std::vector<cell>::iterator it = pro.begin(); it != pro.end(); ++it) {
		if (calculate_critical_field(it->electron_density, it->electron_temperature)
				< it->electric_field)
			return 1;
	}

	return 0;
}

double calculate_critical_field(double electron_density, double electron_temperature) {
	
	//! \a REQ-4: Coulomb logarithm
	/*!
	\f[
		\ln \Lambda = 14.9-0.5 \cdot \log \left(n_e \cdot 10^{-20}\right) + \log \left(t_e \cdot 10^{-3}\right) .
	\f]
	*/
	double coulomb_log = 14.9 - 0.5 * log(electron_density * 1e-20)
			+ log(electron_temperature * 1e-3);


	//! \return \a REQ-3: Critical field
	/*!
	\f[
		E_\mathrm{c} = \frac{n_\mathrm{e} e^3 \ln \Lambda}{4\pi\epsilon_0^2 m_\mathrm{e} c^2}
	\f]
	*/
	return electron_density * e3 * coulomb_log / pi_e02_me_4_c2;
}



double dreicer_generation_rate(double electron_density, double electron_temperature,
		double effective_charge, double electric_field) {
		
		
	//! electron temperature: electronvolt to joule
	/*!
	\f[
		T_\mathrm{e}~\mathrm{[J]} = e \cdot t_\mathrm{e}~\mathrm{[eV]}
	\f]
	*/	
		
	double tej = electron_temperature* ITM_EV;
	
	//! \a REQ-3: Coulomb logarithm
	/*!
	\f[
		\ln \Lambda = 14.9-0.5 \cdot \log \left(n_\mathrm{e} \cdot 10^{-20}\right) + \log \left(t_\mathrm{e} \cdot 10^{-3}\right) .
	\f]
	*/
	double coulomb_log = 14.9 - 0.5 * log(electron_density * 1e-20)
			+ log(electron_temperature * 1e-3);


	cout << "Coulomb logarithm: " << coulomb_log << "\n";



	//! \a REQ-2: electron collision time
	/*! 
	\f[
		\tau = 4 \pi \epsilon_0^2 \cdot \frac{m_\mathrm{e}^2 \cdot c^3 }{e^4} \cdot \frac{1}{n_\mathrm{e} \ln \Lambda}		
	\f]
	*/
	double tao = pi_4_e02_me2_c3__e4 / (electron_density * coulomb_log);
	
	
	cout << "Electron collision time: " << tao << " s\n";

	//! \a REQ-1: Dreicer field
		/*!
	\f[
		E_D = \frac{m_\mathrm{e}^2 c^3}{e\tau \cdot T_\mathrm{e}~\mathrm{[J]}}		
	\f]
	*/
	double Ed = me2_c3__e / (tao * tej);
	double Edc = Ed/electric_field;
	
	cout << "Dreicer field: " << Ed << " V/m\n";
	cout << "normalised Dreicer field: " << Edc << "\n";


	//! \a REQ-7: alpha
	/*!
	\f[	
		\alpha = \frac{E}{E_\mathrm{D}} \cdot \frac{m_\mathrm{e} \cdot c^2}{T_\mathrm{e}~\mathrm{[J]}}
	\f]
	*/
	
	double alpha = electric_field/Ed * me_c2/tej;
	double alpha_2 = alpha*alpha;
	
	
	cout << "alpha: " << alpha << "\n";
	cout << "alpha^2: " << alpha_2 << "\n";
	
	//! \a REQ-6: lambda
	/*!
	\f[	
		\lambda(\alpha)=8\alpha\left( \alpha-\frac{1}{2}-\sqrt{\alpha(\alpha-1}\right)
	\f]
	*/
	double lambda = 8*alpha*(alpha-1/2*sqrt(alpha*(alpha-1)));
	cout << "lambda: " << lambda << "\n";
	
	//! \a REQ-5: multiplication factor
	/*!
	\f[	
		\gamma(\alpha,Z) = \sqrt{\frac{(1+Z)\alpha^2}{8(\alpha-1)}}\cdot \left( \frac{\pi}{2}-\sin^{-1}\left(1-\frac{2}{\alpha} \right) \right)
	\f]
	*/
	double gamma = sqrt((1+effective_charge)*alpha_2/8/(alpha-1))*(ITM_PI/2-asin(1-2/alpha));
	cout << "gamma: " << gamma << "\n";



	//! \a REQ-4: h factor
	/*!	
	\f[	
		h = \frac{1}{16(\alpha - 1)}\cdot \left( \alpha \cdot (Z+1)-Z+7+2\cdot\sqrt{\frac{\alpha}{\alpha -1} \cdot (1+Z)\cdot(\alpha-2)}
		\right)
	\f]
	*/


	double h=1/(16*(alpha-1))*(alpha*(effective_charge+1)-\
		effective_charge+7+2*sqrt(alpha/(alpha-1))*(1+effective_charge)*(alpha-2));
	cout << "h: " << h << "\n";



	//! \return Dreicer generation rate
		/*!
	\f[
	\gamma_\mathrm{D} = n_\mathrm{e} \cdot\frac{1}{\tau} \left(\frac{E_\mathrm{D}}{E} \right)^h(\alpha,Z) \cdot \exp{-\frac{\lambda}{4} \cdot \frac{E_\mathrm{D}}{E} - \sqrt{2  \frac{E_\mathrm{D}}{E}} \gamma(\alpha,Z)}
		
	\f]
	*/	
	
		
	double Cr=1;	
	double dgr = Cr*electron_density/tao*pow(Edc,h)*\
		exp(-lambda/4*Edc-sqrt(2*Edc)*gamma);
		
	cout << "DGR: " << dgr << "\n";
	
	return dgr;

	
	
	
}


