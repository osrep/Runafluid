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



TEST(Control, runafluid_switch_message) {
EXPECT_EQ(reference_runafluid_switch_message_1, runafluid_switch_message(reference_runafluid_switch));
}

TEST(Control, runafluid_switch_message) {								//Új teszt, a ref.értékek még nincsenek kiszámolva
EXPECT_EQ(reference_runafluid_switch_message_2, runafluid_control(reference_ne,reference_rundensity_before, reference_te,
																  reference_Zeff_1, reference_electric_field_1, reference_magnetic_field,
																  reference_timestep, reference_inv_asp_ratio, reference_rho_tor_norm,
																  reference_runafluid_switch,
																  *reference_rate_values));
}

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
																		 reference_rho_tor_norm, reference_modulevar_avalanche));
}

TEST(Avalanche, CalculateToroidicityAvalanche) {                             //Új teszt, még nincsen meg minden ref. érték
EXPECT_EQ(reference_toroidicity_avalanche, calculate_toroidicity_avalanche(reference_inv_asp_ratio,
																		   reference_electric_field,
																		   reference_ne, reference_te,
																		   reference_rho_tor_norm));
}