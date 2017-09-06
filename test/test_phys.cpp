#include <gtest/gtest.h>
#include "../critical_field.h"
#include "../dreicer.h"
#include "../avalanche.h"
#include "../control.h"


const double reference_te = 1e5;
const double reference_ne = 1e21;
const double reference_dreicer_field = 17.452;
const double reference_critical_field = 0.83625;
const double reference_thermal_electron_collision_time = 4.9909e-4;
const double reference_runaway_electron_collision_time = 2.0383e-3;
const double reference_Zeff_1 = 1.0;
const double reference_Zeff_2 = 1.2;
const double reference_electric_field_1 = 1.2;
const double reference_electric_field_2 = 0.8;

const int formula_id_63 = 63;
const int formula_id_66 = 66;
const int formula_id_67 = 67;
const double reference_dreicer_generation_rate_63 = 3.0822e20;
const double reference_dreicer_generation_rate_66 = 5.3092e19;
const double reference_dreicer_generation_rate_67 = 1.2513e15;
const double reference_alpha = reference_electric_field_1;
const double reference_lambda = 2.0169;
const double reference_gamma =0.6173;
const double reference_h = 1.8865;

const double reference_inv_asp_ratio = 0.30303;
const double reference_toroidicity_dreicer = 0.1816067;

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

/*
TEST(Control, runafluid_switch_message_1) {
EXPECT_EQ(reference_runafluid_switch_message_1, runafluid_switch_message(reference_runafluid_switch));
}

TEST(Control, runafluid_switch_message_2) {								//Új teszt, a ref.értékek még nincsenek kiszámolva
EXPECT_EQ(reference_runafluid_switch_message_2, runafluid_control(reference_ne,reference_rundensity_before, reference_te,
																  reference_Zeff_1, reference_electric_field_1, reference_magnetic_field,
																  reference_timestep, reference_inv_asp_ratio, reference_rho_tor_norm,
																  reference_runafluid_switch,
																  *reference_rate_values));
}
*/

TEST(CoulombLog, CalculateCoulombLog) {
EXPECT_NEAR(16.4, calculate_coulomb_log(reference_ne, reference_te), 0.0001);
}

TEST(CriticalField, CalculateCriticalField) {
EXPECT_NEAR(reference_critical_field, calculate_critical_field(reference_ne, reference_te), 0.0001);
}

TEST(GrowthRate, CalculateDreicerField) {
EXPECT_NEAR(reference_dreicer_field, calculate_dreicer_field(reference_thermal_electron_collision_time, reference_te), 0.01);
}

TEST(GrowthRate, CalculateThermalElectronCollisionTime) {
EXPECT_NEAR(reference_thermal_electron_collision_time, calculate_thermal_electron_collision_time(reference_ne,reference_te), 0.0001);
}

TEST(Dreicer, DreicerGenerationRate_63) {
EXPECT_NEAR(reference_dreicer_generation_rate_63, dreicer_generation_rate(reference_ne, reference_te,reference_Zeff_1,reference_electric_field_1,formula_id_63), 1e18);
}

TEST(Dreicer, DreicerGenerationRate_66) {
EXPECT_NEAR(reference_dreicer_generation_rate_66, dreicer_generation_rate(reference_ne, reference_te, reference_Zeff_1,reference_electric_field_1,formula_id_66), 1e18);
}

TEST(Dreicer, DreicerGenerationRate_67) {
EXPECT_NEAR(reference_dreicer_generation_rate_67, dreicer_generation_rate(reference_ne, reference_te, reference_Zeff_1,reference_electric_field_1,formula_id_67), 1e18);
}

TEST(Dreicer, CalculateAlpha) {
EXPECT_EQ(reference_alpha,calculate_alpha(reference_electric_field_1, reference_ne,reference_te));
}

TEST(Dreicer, CalculateLambda) {
EXPECT_EQ(reference_lambda,calculate_lambda(reference_alpha));
}

TEST(Dreicer, CalculateGamma) {
EXPECT_EQ(reference_gamma,calculate_gamma(reference_Zeff_1, reference_alpha));
}

TEST(Dreicer, CalculateH) {
EXPECT_EQ(reference_h,calculate_h(reference_alpha, reference_Zeff_1));
}

TEST(Dreicer, CalculateToroidicityDreicer) {
EXPECT_NEAR(reference_toroidicity_dreicer,calculate_toroidicity_dreicer(reference_inv_asp_ratio),1e18);
}


TEST(Avalanche, CalculateAvalancheGenerationRate) {                             //Új teszt, még nincsen meg minden ref. érték
EXPECT_EQ(reference_avalanche_generation_rate, avalanche_generation_rate(reference_ne, reference_te, reference_Zeff_1,
																		 reference_electric_field_1,reference_magnetic_field,
																		 reference_modulevar_avalanche));
}

TEST(Avalanche, CalculateToroidicityAvalanche) {                             //Új teszt, még nincsen meg minden ref. érték
EXPECT_EQ(reference_toroidicity_avalanche, calculate_toroidicity_avalanche(reference_inv_asp_ratio,
																		   reference_electric_field,
																		   reference_ne, reference_te,
																		   reference_rho_tor_norm));
}