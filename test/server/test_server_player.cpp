#include <boost/test/unit_test.hpp>

#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Player_Server)
{
    server::Player player("Name", state::Player_Type::Human, "session_token");
    // Testing initialization
    BOOST_CHECK_EQUAL(player.get_name(), "Name");
    BOOST_CHECK_EQUAL(player.get_player_type(), state::Player_Type::Human);
    BOOST_CHECK_EQUAL(player.get_session_token(), "session_token");
    BOOST_CHECK_EQUAL(player.get_room(), -1);
    // Testing methods
    player.set_room(0);
    BOOST_CHECK_EQUAL(player.get_room(), 0);
    player.set_last_seen(std::chrono::steady_clock::now());
    BOOST_CHECK(player.get_last_seen() <= std::chrono::steady_clock::now());
}