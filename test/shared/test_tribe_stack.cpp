// tests/test_tribe_stack.cpp
#include <state.h>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Tribe_Stack_Test)
{
    state::Tribe_Stack ts;
    auto               top = ts.get_tribes_on_top();
    BOOST_CHECK_EQUAL(top.size(), 6);

    for (auto* t : top)
    {
        BOOST_CHECK(t != nullptr);
    }

    BOOST_CHECK_THROW(ts.take_tribe_at_position(6), std::out_of_range);

    // taking all tribes out of the stack
    for (size_t i = 0; i < 14; ++i)
    {
        state::Tribe* p = ts.take_tribe_at_position(0);
        BOOST_CHECK(p != nullptr);
    }

    BOOST_CHECK_THROW(ts.take_tribe_at_position(0), std::out_of_range);
}
