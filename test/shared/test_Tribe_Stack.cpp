// tests/test_tribe_stack.cpp
#include <boost/test/included/unit_test.hpp>
#include "Tribe_Stack.h"

using namespace state;

BOOST_AUTO_TEST_CASE(TribeStack_top_has_seeable_count)
{
    Tribe_Stack ts;
    auto top = ts.get_tribes_on_top();
    // seeable_stack_number is 6 -> expect 7 entries
    BOOST_CHECK_EQUAL(top.size(), 7);
    for (auto* t : top) {
        BOOST_CHECK(t != nullptr);
    }
}

BOOST_AUTO_TEST_CASE(TribeStack_take_invalid_high_throws)
{
    Tribe_Stack ts;
    BOOST_CHECK_THROW(ts.take_tribe_at_position(7), std::out_of_range); // > seeable_stack_number
}

BOOST_AUTO_TEST_CASE(TribeStack_take_invalid_negative_throws)
{
    Tribe_Stack ts;
    // If implementation is hardened, this should throw. If not, this test will expose the bug.
    BOOST_CHECK_THROW(ts.take_tribe_at_position(-1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TribeStack_take_valid_positions_no_throw_and_nonnull)
{
    Tribe_Stack ts;
    for (int i = 0; i <= 6; ++i) {
        BOOST_CHECK_NO_THROW({
            Tribe* p = ts.take_tribe_at_position(0); // always take from front to avoid shifting issues
            BOOST_CHECK(p != nullptr);
        });
    }
}
