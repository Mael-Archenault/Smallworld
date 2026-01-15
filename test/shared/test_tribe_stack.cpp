// tests/test_tribe_stack.cpp
#include <state.h>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Tribe_Stack_Test)
{
    state::Tribe_Stack ts(1);
    auto               top = ts.get_tribes_on_top();
    BOOST_CHECK_EQUAL(top.size(), 6);

    for (auto* t : top)
    {
        BOOST_CHECK(t != nullptr);
    }

    BOOST_CHECK_THROW(ts.take_tribe_at_position(6), std::out_of_range);

    // trying the infinite tribe generation
    int iterations = 100;
    for (int i = 0; i < iterations; i++)
    {
        state::Tribe* p = ts.take_tribe_at_position(0);
        BOOST_CHECK(p != nullptr);
        ts.remove_from_in_game_tribes(p->id);
    }
}
