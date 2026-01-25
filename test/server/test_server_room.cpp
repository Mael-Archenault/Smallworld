#include <boost/test/unit_test.hpp>

#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Room_Server)
{
    server::Player owner("Owner", state::Player_Type::Human, "owner_token");
    server::Room   room(0, owner);

    // Testing initialization
    BOOST_CHECK_EQUAL(room.id, 0);
    BOOST_CHECK_EQUAL(room.get_owner().get_name(), "Owner");
    BOOST_CHECK_EQUAL(room.get_owner().get_session_token(), "owner_token");
    BOOST_CHECK_EQUAL(room.get_state(), server::Room_State::WAITING);
    BOOST_CHECK_EQUAL(room.is_empty(), false);
    BOOST_CHECK_EQUAL(room.is_full(), false);

    // Testing errors
    BOOST_CHECK_THROW(
        room.remove_player(server::Player("NonMember", state::Player_Type::Human, "token")),
        std::runtime_error);
    BOOST_CHECK_THROW(room.add_player(owner), std::runtime_error);

    // Testing adding an ai
    server::Player player1("Player1", state::Player_Type::Random_AI, "token1");
    room.add_player(player1);
    BOOST_CHECK_EQUAL(room.is_empty(), false);

    // Testing removing the ai
    room.remove_ai();

    // Testing infos
    server::Player player2("Player2", state::Player_Type::Human, "token2");
    room.add_player(player2);
    room.get_start_infos();
    room.get_infos("owner_token");
    room.get_infos("token2");

    // Testing removing owner

    room.remove_player(owner);
    BOOST_CHECK_EQUAL(room.get_owner().get_name(), "Player2");
    // Testing set_state
    room.set_state(server::Room_State::IN_GAME);
    BOOST_CHECK_EQUAL(room.get_state(), server::Room_State::IN_GAME);

    room.remove_player(player2);
    BOOST_CHECK_EQUAL(room.is_empty(), true);
}