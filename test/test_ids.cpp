#include <gtest/gtest.h>
#include "dreicer.h"
#include "ids_utils.h"
#include "distinit.h"

IdsNs::IDS::core_profiles coreprof;
IdsNs::IDS::equilibrium equilibrium;
IdsNs::IDS::distributions distribution;

const int timeindex = 0;
const double RefNumberIsPositive = 1.0;
const double RefNumberIsZero = 0.0;
const double RefNumberIsNegative = -1.0;
const double reference_negative = -1.5;
const double reference_zero = 0;
const double reference_positive = 1.5;

TEST(IdsFunc, sign) {
	EXPECT_EQ(RefNumberIsPositive, sign(reference_positive));
	EXPECT_EQ(RefNumberIsZero, sign(reference_zero));
	EXPECT_EQ(RefNumberIsNegative, sign(reference_negative));
}


void create_ids() {
	std::size_t size = 5;
	coreprof.profiles_1d.resize(1);

	coreprof.profiles_1d(timeindex).grid.rho_tor.resize(size);
	coreprof.profiles_1d(timeindex).grid.rho_tor = 0.0, 1.0, 2.0, 4.0, 8.0;

	coreprof.profiles_1d(timeindex).grid.rho_tor_norm.resize(size);
	coreprof.profiles_1d(timeindex).grid.rho_tor_norm = 0.0, 0.1, 0.2, 0.4, 0.8;

	coreprof.profiles_1d(timeindex).electrons.density.resize(size);
	coreprof.profiles_1d(timeindex).electrons.density = 10.0, 11.0, 12.0, 14.0, 18.0;

	coreprof.profiles_1d(timeindex).electrons.temperature.resize(size);
	coreprof.profiles_1d(timeindex).electrons.temperature = 20.0, 21.0, 22.0, 24.0, 28.0;

	coreprof.profiles_1d(timeindex).e_field.parallel.resize(size);
	coreprof.profiles_1d(timeindex).e_field.parallel = 1.0, 2.0, 3.0, 5.0, 9.0;

	//effective charge
	coreprof.profiles_1d(timeindex).zeff.resize(size);
	coreprof.profiles_1d(timeindex).zeff = 1.0, 1.0, 1.0, 1.0, 1.0;
	
	equilibrium.time_slice.resize(1);

	equilibrium.time_slice(timeindex).profiles_1d.b_field_average.resize(5);
	equilibrium.time_slice(timeindex).profiles_1d.rho_tor.resize(5);
	equilibrium.time_slice(timeindex).profiles_1d.b_field_average = 1.0, 1.0, 1.0, 1.0, 1.0;
	equilibrium.time_slice(timeindex).profiles_1d.rho_tor = 1.0, 1.0, 1.0, 1.0, 1.0;
}
TEST(IdsFunc, whereRunaway){
	create_ids();
	distinit(distribution, coreprof, timeindex);	
	EXPECT_EQ(0, whereRunaway(distribution));
}

TEST(IdsToProfile, profileSize){
	create_ids();
	distinit(distribution, coreprof, timeindex);
	plasma_profile plasmaProfile = ids_to_profile(coreprof, equilibrium, distribution, timeindex);
	EXPECT_EQ(5, plasmaProfile.size());
}

TEST(IdsToProfile, ElectronDensity) {
	create_ids();
	distinit(distribution, coreprof, timeindex);
	plasma_profile plasmaProfile = ids_to_profile(coreprof, equilibrium, distribution, timeindex);
	EXPECT_DOUBLE_EQ(10.0, plasmaProfile[0].electron_density);
	EXPECT_DOUBLE_EQ(11.0, plasmaProfile[1].electron_density);
	EXPECT_DOUBLE_EQ(12.0, plasmaProfile[2].electron_density);
	EXPECT_DOUBLE_EQ(14.0, plasmaProfile[3].electron_density);
	EXPECT_DOUBLE_EQ(18.0, plasmaProfile[4].electron_density);

}
TEST(IdsToProfile, ElectronTemperature) {
	create_ids();
	distinit(distribution, coreprof, timeindex);
	plasma_profile plasmaProfile = ids_to_profile(coreprof, equilibrium, distribution, timeindex);
	EXPECT_DOUBLE_EQ(20.0, plasmaProfile[0].electron_temperature);
	EXPECT_DOUBLE_EQ(21.0, plasmaProfile[1].electron_temperature);
	EXPECT_DOUBLE_EQ(22.0, plasmaProfile[2].electron_temperature);
	EXPECT_DOUBLE_EQ(24.0, plasmaProfile[3].electron_temperature);
	EXPECT_DOUBLE_EQ(28.0, plasmaProfile[4].electron_temperature);
}
TEST(IdsToProfile, ElectricField) {
	create_ids();
	distinit(distribution, coreprof, timeindex);
	plasma_profile pro = ids_to_profile(coreprof, equilibrium, distribution, timeindex);
	EXPECT_DOUBLE_EQ(1.0, pro[0].electric_field);
	EXPECT_DOUBLE_EQ(2.0, pro[1].electric_field);
	EXPECT_DOUBLE_EQ(3.0, pro[2].electric_field);
	EXPECT_DOUBLE_EQ(5.0, pro[3].electric_field);
	EXPECT_DOUBLE_EQ(9.0, pro[4].electric_field);
}
TEST(IdsToProfile, EffectiveCharge) {
	create_ids();
	distinit(distribution, coreprof, timeindex);
	plasma_profile plasmaProfile = ids_to_profile(coreprof, equilibrium, distribution, timeindex);
	EXPECT_NEAR(1.0, plasmaProfile[0].effective_charge, 0.00001);
	EXPECT_NEAR(1.0, plasmaProfile[1].effective_charge, 0.00001);
	EXPECT_NEAR(1.0, plasmaProfile[2].effective_charge, 0.00001);
	EXPECT_NEAR(1.0, plasmaProfile[3].effective_charge, 0.00001);
	EXPECT_NEAR(1.0, plasmaProfile[4].effective_charge, 0.00001);
}