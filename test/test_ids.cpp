#include <gtest/gtest.h>
#include "dreicer.h"


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
