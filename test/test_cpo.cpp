#include <gtest/gtest.h>
#include "../cpo_utils.h"


ItmNs::Itm::coreprof coreprof;
ItmNs::Itm::coreimpur coreimpur;
ItmNs::Itm::equilibrium equilibrium;
ItmNs::Itm::distribution distribution;

double reference_Number = -1.5;												//Referencia szám a teszteléshez
double RefNumberIsPositive = 1.0;
double RefNumberIsZero = 0.0;
double RefNumberIsNegative = -1.0;


TEST(Equal, Tolerance) {
EXPECT_TRUE(equal(1.0, 1.1, 0.1));
EXPECT_TRUE(equal(-10.0, -11.0, 0.1));
EXPECT_TRUE(equal(-10.0, 10.0, 2.0));
EXPECT_FALSE(equal(1.0, 1.2, 0.1));
EXPECT_FALSE(equal(-10.0, -12.0, 0.1));
}

TEST(CpoFunc, bool_switch) {												//Új teszt, még nincsenek meg hozzá a
EXPECT_EQ(reference_bool, bool_switch(switch_numbers, *bools, N ));			//referenciaértékek
}

TEST(CpoFunc, sign) {														//Új teszt, ezek közül kettőnek failelni kell,
EXPECT_EQ(RefNumberIsPositive, sign(reference_Number));						//hogyha helyesen működik a sign fv.
EXPECT_EQ(RefNumberIsZero, sign(reference_Number));
EXPECT_EQ(RefNumberIsNegative, sign(reference_Number));
}

TEST(CpoFunc, get_digit) {													//Új teszt, még nincsenek meg hozzá a
EXPECT_EQ(reference_digit, get_digit(reference_number, reference_digit));	//referenciaértékek
}

TEST(CpoFunc, whereRunaway) {												//Új teszt, még nincsenek meg hozzá a
EXPECT_EQ(reference_runaway_index, whereRunaway(*reference_distribution));	//referenciaértékek, és a ref_dist-et sem
}																			//tudom, mi pontosan

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

void create_cpo() {
	coreprof.rho_tor.resize(5);
	coreprof.rho_tor = 0.0, 1.0, 2.0, 4.0, 8.0;
	coreprof.rho_tor_norm.resize(5);
	coreprof.rho_tor_norm = 0.0, 0.1, 0.2, 0.4, 0.8;

	coreprof.ne.value.resize(5);
	coreprof.te.value.resize(5);
	coreprof.ne.value = 10.0, 11.0, 12.0, 14.0, 18.0;
	coreprof.te.value = 20.0, 21.0, 22.0, 24.0, 28.0;

	coreprof.toroid_field.b0 = 2.0;

	coreprof.profiles1d.eparallel.value.resize(5);
	coreprof.profiles1d.eparallel.value = 1.0, 2.0, 3.0, 5.0, 9.0;

	equilibrium.profiles_1d.rho_tor.resize(8);
	equilibrium.profiles_1d.rho_tor = 0.0, 1.0, 1.5, 2.0, 5.0, 10.0, 12.0, 15.0;
	equilibrium.profiles_1d.b_av.resize(8);
	equilibrium.profiles_1d.b_av = 5.5, 1.5, 5.5, 2.5, 1.6, 11.6, 15.5, 55.5;

	coreprof.ni.value.resize(5, 2);
	coreprof.ni.value = 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;

	coreprof.compositions.ions.resize(2);
	coreprof.compositions.ions(0).zion = 2.0;
	coreprof.compositions.ions(1).zion = 3.0;

	coreimpur.rho_tor.resize(6);
	coreimpur.rho_tor = 0.0, 1.0, 1.5, 2.0, 6.0, 10.0;

	coreimpur.impurity.resize(2);
	coreimpur.impurity(0).z.resize(6, 2);
	coreimpur.impurity(0).z = 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24;
	coreimpur.impurity(1).z.resize(6, 2);
	coreimpur.impurity(1).z = 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;

	coreimpur.impurity(0).nz.resize(6, 2);
	coreimpur.impurity(0).nz = 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2;
	coreimpur.impurity(1).nz.resize(6, 2);
	coreimpur.impurity(1).nz = 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;

	distribution.distri_vec.resize(1);
	distribution.distri_vec(0).profiles_1d.state.dens.resize(6);
	distribution.distri_vec(0).profiles_1d.state.current.resize(6);
	distribution.distri_vec(0).source_id.resize(1);
	//! Filling up distribution source
	distribution.distri_vec(0).source_id(0).type.id = "runaway";
	distribution.distri_vec(0).source_id(0).type.flag = 7;
	distribution.distri_vec(0).source_id(0).type.description = "Source from runaway processes";
	//! Filling up distribution species
	distribution.distri_vec(0).species.type.id = "electron";
	distribution.distri_vec(0).species.type.flag = 1;
	distribution.distri_vec(0).species.type.description = "Electron";
	distribution.distri_vec(0).gyro_type = 1;
	distribution.distri_vec(0).profiles_1d.geometry.rho_tor.resize(6);
	distribution.distri_vec(0).profiles_1d.geometry.rho_tor_norm.resize(6);
	distribution.distri_vec(0).profiles_1d.geometry.rho_tor = 0.0, 1.0, 1.5, 2.0, 6.0, 10.0;
	distribution.distri_vec(0).profiles_1d.geometry.rho_tor_norm = 0.0, 0.10, 0.15, 0.20, 0.60, 1.0;
}

TEST(CpoToProfil, ElectronDensity) {
create_cpo();
profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);

ASSERT_EQ(5, pro.size());

EXPECT_DOUBLE_EQ(10.0, pro[0].electron_density);
EXPECT_DOUBLE_EQ(11.0, pro[1].electron_density);
EXPECT_DOUBLE_EQ(12.0, pro[2].electron_density);
EXPECT_DOUBLE_EQ(14.0, pro[3].electron_density);
EXPECT_DOUBLE_EQ(18.0, pro[4].electron_density);
}

TEST(CpoToProfil, ElectronTemperature) {
create_cpo();
profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);

ASSERT_EQ(5, pro.size());

EXPECT_DOUBLE_EQ(20.0, pro[0].electron_temperature);
EXPECT_DOUBLE_EQ(21.0, pro[1].electron_temperature);
EXPECT_DOUBLE_EQ(22.0, pro[2].electron_temperature);
EXPECT_DOUBLE_EQ(24.0, pro[3].electron_temperature);
EXPECT_DOUBLE_EQ(28.0, pro[4].electron_temperature);
}

TEST(CpoToProfil, ElectricField) {
create_cpo();
profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);

ASSERT_EQ(5, pro.size());

EXPECT_DOUBLE_EQ(1.0 * 2.0 / 5.5, pro[0].electric_field);
EXPECT_DOUBLE_EQ(2.0 * 2.0 / 1.5, pro[1].electric_field);
EXPECT_DOUBLE_EQ(3.0 * 2.0 / 2.5, pro[2].electric_field);
EXPECT_DOUBLE_EQ(5.0 * 2.0 / 1.9, pro[3].electric_field);
EXPECT_DOUBLE_EQ(9.0 * 2.0 / 7.6, pro[4].electric_field);
}

TEST(CpoToProfil, EffectiveCharge) {
create_cpo();
profile pro = cpo_to_profile(coreprof, coreimpur, equilibrium, distribution);

ASSERT_EQ(7, pro.size());

EXPECT_NEAR(3.46000, pro[0].effective_charge, 0.00001);
EXPECT_NEAR(131.24, pro[3].effective_charge, 0.01);
}
