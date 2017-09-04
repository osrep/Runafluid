#include <gtest/gtest.h>
#include "../dreicer.h"



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

ItmNs::Itm::coreprof coreprof;
ItmNs::Itm::coreimpur coreimpur;
ItmNs::Itm::equilibrium equilibrium;
ItmNs::Itm::distribution distribution;

TEST(IdsUtils, get_digit) {													//Új teszt, még nincsenek meg hozzá a
EXPECT_EQ(reference_got_digit, get_digit(reference_number, reference_digit));	//referenciaértékek
}

TEST(IdsUtils, whereRunaway) {												//Új teszt, még nincsenek meg hozzá a
EXPECT_EQ(reference_runaway_index, whereRunaway(*reference_distribution));	//referenciaértékek, és a ref_dist-et sem
}																			//tudom, mi pontosan

TEST(IdsUtils, IdsToProfile) {												//Új teszt, még nem tudom, hogyan írjam meg

}
