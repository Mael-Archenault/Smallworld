#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Amazons_Species)
{
    effects::Elves_Species elves_species;

    BOOST_CHECK_EQUAL(elves_species.lose_effect(nullptr), 1);
}
