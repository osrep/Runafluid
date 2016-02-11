#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <vector>

//! value of \f$ \pi \f$
#define ITM_PI		3.141592653589793238462643383280

//! speed of light [m/s]
#define ITM_C		2.99792458e8

//! electron volt [eV]
#define ITM_EV		1.602176565e-19

//! elementary charge [coulomb]
#define ITM_QE		ITM_EV

//! \f$ m_e \f$ electron mass [kg]
#define ITM_ME		9.10938291e-31

//! \f$ \mu_0 \f$ vacuum permeability
#define ITM_MU0		4.0e-7 * ITM_PI


//! \f$ \epsilon_0 \f$ vacuum permittivity
#define ITM_EPS0	1.0 / (ITM_MU0 * ITM_C * ITM_C)

//! ERROR CODE
#define ITM_ILLEGAL_INT	-999999999



struct cell {
	double electron_density;
	double electron_temperature;
	double effective_charge;
	double electric_field;
};

typedef std::vector<cell> profile;

#endif /* CONSTANTS_H_ */
