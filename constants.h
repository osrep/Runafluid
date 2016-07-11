#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//! value of \f$ \pi \f$
//#define ITM_PI		3.141592653589793238462643383280
const double ITM_PI = 3.141592653589793238462643383280;

//! speed of light [m/s]
//#define ITM_C		2.99792458e8
const double ITM_C = 2.99792458e8;

//! electron volt [eV]
//#define ITM_EV		1.602176565e-19
const double ITM_EV	= 1.602176565e-19;

//! elementary charge [coulomb]
//#define ITM_QE		ITM_EV
const double ITM_QE = ITM_EV;

//! \f$ m_e \f$ electron mass [kg]
//#define ITM_ME		9.10938291e-31
const double ITM_ME = 9.10938291e-31;

//! \f$ \mu_0 \f$ vacuum permeability
//#define ITM_MU0		4.0e-7 * ITM_PI
const double ITM_MU0 = 4.0e-7 * ITM_PI;

//! \f$ \epsilon_0 \f$ vacuum permittivity
//#define ITM_EPS0	1.0 / (ITM_MU0 * ITM_C * ITM_C)
const double ITM_EPS0 = 1.0 / (ITM_MU0 * ITM_C * ITM_C);


//! distsource_identifier for runaways (7)
//#define DISTSOURCE_IDENTIFIER 7
const int DISTSOURCE_IDENTIFIER = 7;

//! ERROR CODE
//#define ITM_ILLEGAL_INT	-999999999
const int ITM_ILLEGAL_INT = -999999999;


#define ITM_ILLEGAL_FLOAT -9.E40
const double ITM_ILLEGAL_FLOAT = -9.E40;

#endif /* CONSTANTS_H_ */
