#include <gtest/gtest.h>
#include "../critical_field.h"
#include "../dreicer.h"
#include "../avalanche.h"
#include "../control.h"
#ifdef ITM_CONSTANTS
    #include "../cpo_utils.h"
#else
    #include "../ids_utils.h"
#endif

using namespace std;

const double reference_te = 1e5;
const double reference_ne = 1e21;
const double reference_dreicer_field = 4.78235;
const double reference_critical_field = 0.935882;
const double reference_thermal_electron_collision_time = 4.4596e-4;
const double reference_runaway_electron_collision_time = 1.82129e-3;
const double reference_Zeff_1 = 1.0;
const double reference_Zeff_2 = 1.2;
const double reference_alpha_1 = 1.2;
const double reference_alpha_2 = 0.8;

std::string str_dreicer_formula63 = "hc_formula_63";
std::string str_dreicer_formula66 = "hc_formula_66";
std::string str_dreicer_formula67 = "hc_formula_67";
std::string str_avalanche_formula = "rosenbluth_putvinski";
std::string str_avalanche_formula_threshold = "rosenbluth_putvinski_with_threshold";
std::string str_output = "/afs/eufus.eu/g2itmdev/user/g2solasz/public/hdf5/test.h5";
double warning_fraction_limit = 1.0;
double rho_edge_calculation_limit = 0.85;

module_struct modules63 {str_dreicer_formula63, false, str_avalanche_formula, false, str_output, warning_fraction_limit, rho_edge_calculation_limit};
module_struct modules66 {str_dreicer_formula66, false, str_avalanche_formula, false, str_output, warning_fraction_limit, rho_edge_calculation_limit};
module_struct modules67 {str_dreicer_formula67, false, str_avalanche_formula, false, str_output, warning_fraction_limit, rho_edge_calculation_limit};

const double reference_dreicer_generation_rate_63 =4.1378e-2;
const double reference_dreicer_generation_rate_66 = 9.1233;
const double reference_dreicer_generation_rate_67 = 7.1931e1;
const double reference_electric_field_1 =  reference_alpha_1*reference_critical_field;
const double reference_electric_field_2 =  reference_alpha_2*reference_critical_field;
const double reference_lambda = 2.0169;
const double reference_Coulomb_logarithm = 18.3539;
const double reference_gamma =3.086476806;
const double reference_h = 0.175510257;
const double reference_synchrotron_loss_time = 1.28967;
const double reference_inv_asp_ratio = 0.30303;
const double reference_toroidicity_dreicer = 0.311578581;

const double reference_rho_tor_norm = 0.65;
const double reference_avalanche_threshold_field = 1.03463;

module_struct modules_no_threshold{str_dreicer_formula63, false, str_avalanche_formula, false, str_output, warning_fraction_limit, rho_edge_calculation_limit};
module_struct modules_threshold{str_dreicer_formula63, false, str_avalanche_formula_threshold, false, str_output, warning_fraction_limit, rho_edge_calculation_limit};

const double reference_magnetic_field = 2;
const int avalanche_generation_rate_mod_1 = 0;
const int avalanche_generation_rate_mod_2 = 0;
const double avalanche_generation_rate_mod_3 = 2.991547;

TEST(Equal, Tolerance) {
	EXPECT_TRUE(equal(1.0, 1.1, 0.1));
	EXPECT_TRUE(equal(-10.0, -11.0, 0.1));
	EXPECT_TRUE(equal(-10.0, 10.0, 2.0));
	EXPECT_FALSE(equal(1.0, 1.2, 0.1));
	EXPECT_FALSE(equal(-10.0, -12.0, 0.1));
}

TEST(BinarySearch, FindExistingValue) {
	blitz::Array<double, 1> ar(10);
	ar = 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
	EXPECT_EQ(0, binary_search(ar, 0.0));
	EXPECT_EQ(1, binary_search(ar, 1.0));
	EXPECT_EQ(2, binary_search(ar, 2.0));
	EXPECT_EQ(3, binary_search(ar, 3.0));
	EXPECT_EQ(4, binary_search(ar, 4.0));
	EXPECT_EQ(5, binary_search(ar, 5.0));
	EXPECT_EQ(6, binary_search(ar, 6.0));
	EXPECT_EQ(7, binary_search(ar, 7.0));
	EXPECT_EQ(8, binary_search(ar, 8.0));
	EXPECT_EQ(8, binary_search(ar, 9.0));
}

TEST(BinarySearch, FindValue) {
	blitz::Array<double, 1> ar(10);
	ar = 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
	EXPECT_EQ(0, binary_search(ar, 0.01));
	EXPECT_EQ(0, binary_search(ar, 0.99));
	EXPECT_EQ(1, binary_search(ar, 1.01));
	EXPECT_EQ(1, binary_search(ar, 1.99));
	EXPECT_EQ(6, binary_search(ar, 6.1));
	EXPECT_EQ(6, binary_search(ar, 6.5));
	EXPECT_EQ(7, binary_search(ar, 7.2));
	EXPECT_EQ(8, binary_search(ar, 8.01));
	EXPECT_EQ(8, binary_search(ar, 8.99));
}

TEST(Interpolate, Matching) {
	blitz::Array<double, 1> x(10), y(10);
	x = 0.0, 1.0, 2.0, 3.0, 4.0, 4.2, 4.6, 8.0, 8.5, 9.0;
	y = 0.0, 1.0, 2.0, 4.0, 8.0, 4.0, 2.0, 0.0, -2.0, 0.0;

	for (int i = 0; i < x.rows(); i++) {
		EXPECT_DOUBLE_EQ(y(i), interpolate(x, y, x(i)));
	}
}

TEST(Interpolate, Extrapolate) {
	blitz::Array<double, 1> x(10), y(10);
	x = 0.0, 1.0, 2.0, 3.0, 4.0, 4.2, 4.6, 8.0, 8.5, 9.0;
	y = 0.0, 1.0, 2.0, 4.0, 8.0, 4.0, 2.0, 0.0, -2.0, -1.0;
	EXPECT_DOUBLE_EQ(0.0, interpolate(x, y, -1.2));
	EXPECT_DOUBLE_EQ(0.0, interpolate(x, y, -100.2));
	EXPECT_DOUBLE_EQ(-1.0, interpolate(x, y, 9.1));
	EXPECT_DOUBLE_EQ(-1.0, interpolate(x, y, 900.1));

}

TEST(Interpolate, Intrapolate) {
	blitz::Array<double, 1> x(10), y(10);
	x = 0.0, 1.0, 2.0, 3.0, 4.0, 4.2, 4.6, 8.0, 8.5, 9.0;
	y = 0.0, 1.0, 2.0, 4.0, 8.0, 4.0, 2.0, 0.0, -2.0, 0.0;
	EXPECT_NEAR(0.5, interpolate(x, y, 0.5), 0.000001);
	EXPECT_NEAR(1.5, interpolate(x, y, 1.5), 0.000001);
	EXPECT_NEAR(3.0, interpolate(x, y, 2.5), 0.000001);
	EXPECT_NEAR(4.4, interpolate(x, y, 3.1), 0.000001);
	EXPECT_NEAR(7.6, interpolate(x, y, 3.9), 0.000001);
	EXPECT_NEAR(5.0, interpolate(x, y, 4.15), 0.000001);
	EXPECT_NEAR(3.75, interpolate(x, y, 4.25), 0.000001);
	EXPECT_NEAR(1.0, interpolate(x, y, 6.3), 0.000001);
	EXPECT_NEAR(-1.6, interpolate(x, y, 8.4), 0.000001);
	EXPECT_NEAR(-1.0, interpolate(x, y, 8.75), 0.000001);
}

TEST(CoulombLog, CalculateCoulombLog) {
	EXPECT_NEAR(reference_Coulomb_logarithm, calculate_coulomb_log(reference_ne, reference_te), reference_Coulomb_logarithm*1e-5);
}

TEST(CriticalField, CalculateCriticalField) {
	EXPECT_NEAR(reference_critical_field, calculate_critical_field(reference_ne, reference_te), reference_critical_field*1e-6);
}

TEST(CriticalField, CalculateRunawayCollisionTime) {
	EXPECT_NEAR(reference_runaway_electron_collision_time, calculate_runaway_collision_time(reference_ne, reference_te), reference_runaway_electron_collision_time*1e-4);
}

TEST(CriticalField, CalculateSynchrotronLossTime) {
	EXPECT_NEAR(reference_synchrotron_loss_time, calculate_synchrotron_loss_time(reference_magnetic_field), reference_synchrotron_loss_time*1e-5);
}

TEST(GrowthRate, CalculateDreicerField) {
	EXPECT_NEAR(reference_dreicer_field, calculate_dreicer_field(reference_ne, reference_te), reference_dreicer_field*1e-5);
}

TEST(GrowthRate, CalculateThermalElectronCollisionTime) {
	EXPECT_NEAR(reference_thermal_electron_collision_time, calculate_thermal_electron_collision_time(reference_ne,reference_te), reference_thermal_electron_collision_time*1e-4);
}

TEST(Dreicer, DreicerGenerationRate_63) {
	EXPECT_NEAR(reference_dreicer_generation_rate_63, dreicer_generation_rate(reference_ne, reference_te,reference_Zeff_1,
																		  reference_electric_field_1,reference_rho_tor_norm,modules63), reference_dreicer_generation_rate_63*1e-4);
}

TEST(Dreicer, DreicerGenerationRate_66) {
	EXPECT_NEAR(reference_dreicer_generation_rate_66, dreicer_generation_rate(reference_ne, reference_te, reference_Zeff_1,
																		  reference_electric_field_1,reference_rho_tor_norm,modules66), reference_dreicer_generation_rate_66*1e-4);
}

TEST(Dreicer, DreicerGenerationRate_67) {
	EXPECT_NEAR(reference_dreicer_generation_rate_67, dreicer_generation_rate(reference_ne, reference_te, reference_Zeff_1,
																		  reference_electric_field_1,reference_rho_tor_norm,modules67), reference_dreicer_generation_rate_67*1e-4);
}

TEST(Dreicer, CalculateLambda) {
	EXPECT_NEAR(reference_lambda,calculate_lambda(reference_alpha_1),reference_lambda*1e-4);
}

TEST(Dreicer, CalculateGamma) {
	EXPECT_NEAR(reference_gamma,calculate_gamma(reference_Zeff_1, reference_alpha_1),reference_gamma*1e-9);
}

TEST(Dreicer, CalculateH) {
	EXPECT_NEAR(reference_h,calculate_h(reference_alpha_1, reference_Zeff_1), reference_h*1e-8);
}

TEST(Dreicer, CalculateToroidicityDreicer) {
	EXPECT_NEAR(reference_toroidicity_dreicer,calculate_toroidicity_dreicer(reference_inv_asp_ratio,reference_rho_tor_norm),reference_toroidicity_dreicer*1e-6);
}

TEST(Avalanche, CalculateAvalancheThresholdField){								//new test with good inputs and outputs
	EXPECT_NEAR(reference_avalanche_threshold_field, calculate_avalanche_threshold_field(reference_ne, reference_te, reference_Zeff_1, reference_critical_field, reference_magnetic_field),reference_avalanche_threshold_field*1e-5);
}


TEST(Avalanche, CalculateAvalancheGenerationRate) {

	EXPECT_NEAR(avalanche_generation_rate_mod_1, avalanche_generation_rate(reference_ne, reference_te, reference_Zeff_1,
																		 reference_electric_field_2,reference_magnetic_field,
																		 modules_no_threshold), avalanche_generation_rate_mod_1*1e-6);
	EXPECT_NEAR(avalanche_generation_rate_mod_2, avalanche_generation_rate(reference_ne, reference_te, reference_Zeff_1,
																		 reference_electric_field_2,reference_magnetic_field,
																		 modules_no_threshold), avalanche_generation_rate_mod_2*1e-6);
	EXPECT_NEAR(avalanche_generation_rate_mod_3, avalanche_generation_rate(reference_ne, reference_te, reference_Zeff_1,
																		 reference_electric_field_1,reference_magnetic_field,
																		 modules_threshold), avalanche_generation_rate_mod_3*1e-5);

}
/*
TEST(Avalanche, CalculateToroidicityAvalanche) {                             //Új teszt, még nincsen meg minden ref. érték
	EXPECT_EQ(reference_toroidicity_avalanche, calculate_toroidicity_avalanche(reference_inv_asp_ratio,
																		 	   reference_electric_field,
																			   reference_ne, reference_te,
																			   reference_rho_tor_norm));
}
*/
