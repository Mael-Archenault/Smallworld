// tests/test_tribe_stack.cpp
#include <boost/test/included/unit_test.hpp>
#include <state.h>


BOOST_AUTO_TEST_CASE(TribeStack_top_has_seeable_count)
{
    state::Tribe_Stack ts;
    auto top = ts.get_tribes_on_top();
    BOOST_CHECK_EQUAL(top.size(), 6);
    for (auto* t : top) {
        BOOST_CHECK(t != nullptr);
    }
}

BOOST_AUTO_TEST_CASE(TribeStack_take_invalid_high_throws)
{
    state::Tribe_Stack ts;
    BOOST_CHECK_THROW(ts.take_tribe_at_position(6), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TribeStack_take_valid_positions_no_throw_and_nonnull)
{
    state::Tribe_Stack ts;
    for (int i = 0; i <= 6; ++i) {
        BOOST_CHECK_NO_THROW({
            state::Tribe* p = ts.take_tribe_at_position(0); // always take from front to avoid shifting issues
            BOOST_CHECK(p != nullptr);
        });
    }
}
