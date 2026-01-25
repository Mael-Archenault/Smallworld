#include <boost/test/unit_test.hpp>

#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Player_Manager)
{
    server::Player_Manager* player_manager = new server::Player_Manager();
    server::Room_Service*   room_service   = new server::Room_Service(*player_manager);
    player_manager->set_room_service_reference(room_service);
    // Testing initialization
    BOOST_CHECK_EQUAL(player_manager->get_sessions_update_thread_running(), true);

    // Testing create_player
    std::string session_token = player_manager->create_player("TestPlayer");
    BOOST_CHECK(!session_token.empty());

    // Testing verify_session_token
    BOOST_CHECK_EQUAL(player_manager->verify_session_token(session_token), true);
    BOOST_CHECK_EQUAL(player_manager->verify_session_token("invalid_token"), false);

    // Testing get_player
    server::Player& player = player_manager->get_player(session_token);
    BOOST_CHECK_EQUAL(player.get_name(), "TestPlayer");
    BOOST_CHECK_EQUAL(player.get_room(), -1);
    BOOST_CHECK_THROW(player_manager->get_player("invalid_token"), std::runtime_error);

    // Testing refresh_last_seen
    auto time_before = player.get_last_seen();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    player_manager->refresh_last_seen(session_token);
    auto time_after = player.get_last_seen();
    BOOST_CHECK(time_after >= time_before);

    usleep(10000000);  // wait a bit to let the thread update once and remove the player
    // Testing remove_inactive_players
    BOOST_CHECK_THROW(player_manager->get_player(session_token), std::runtime_error);

    delete player_manager;
}
