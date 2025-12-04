
#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Amazons_Species)
{
    effects::Amazons_Species amazons_species;

    BOOST_CHECK_EQUAL(amazons_species.first_gather_effect(5), 9);
    BOOST_CHECK_EQUAL(amazons_species.first_gather_effect(13), 15);

    BOOST_CHECK_EQUAL(amazons_species.second_gather_effect(5), 1);
}
