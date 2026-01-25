#include <boost/test/unit_test.hpp>

#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Room_Service)
{
    server::Player_Manager                player_manager;
    std::shared_ptr<server::Room_Service> room_service =
        std::make_shared<server::Room_Service>(player_manager);
    std::shared_ptr<server::Game_Service> game_service =
        std::make_shared<server::Game_Service>(room_service);
    // Testing initialization
    BOOST_CHECK_EQUAL(room_service->get_root_prefix(), "/rooms");
    room_service->set_ref_to_game_service(game_service);

    std::string in;
    std::string out;
    // Testing create_room
    std::string         owner_session_token = player_manager.create_player("TestCreator");
    server::Http_Status status = room_service->post(in, out, "/create", owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::CREATED);
    int room_id = std::stoi(out);

    status = room_service->post(in, out, "/create", "invalid_token");
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);

    status = room_service->post(in, out, "/create", owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);  // already in a room

    // Testing joining a room

    std::string joiner_session_token = player_manager.create_player("TestJoiner");
    status = room_service->post(in, out, "/join/" + std::to_string(room_id), joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    status = room_service->post(in, out, "/join/999", joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::NOT_FOUND);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    status = room_service->post(in, out, "/join/" + std::to_string(room_id), "invalid_token");
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    status = room_service->post(in, out, "/join/" + std::to_string(room_id), joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);  // already in a room
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    // Testing exiting a room
    status = room_service->post(in, out, "/exit/" + std::to_string(room_id), joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    status = room_service->post(in, out, "/exit/" + std::to_string(room_id), "invalid_token");
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    status = room_service->post(in, out, "/exit/" + std::to_string(room_id), joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);  // not in the room
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    // Testing adding an AI
    in     = "1,ai_name";
    status = room_service->post(in, out, "/add_ai/" + std::to_string(room_id), owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);
    status =
        room_service->post(in, out, "/add_ai/" + std::to_string(room_id), joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);
    in     = "99,ai_name";  // not existing ai type
    status = room_service->post(in, out, "/add_ai/" + std::to_string(room_id), owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    // Testing removing an AI
    status =
        room_service->post(in, out, "/delete_ai/" + std::to_string(room_id), owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    status =
        room_service->post(in, out, "/delete_ai/" + std::to_string(room_id), joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    // Testing unknown methods
    status = room_service->post(in, out, "/unknown_action/" + std::to_string(room_id),
                                owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::METHOD_NOT_ALLOWED);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    status = room_service->get(in, out, "/unknown_action/" + std::to_string(room_id),
                               owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::METHOD_NOT_ALLOWED);

    // Testing getting lobby state

    status = room_service->get(in, out, "/state/" + std::to_string(room_id), owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    // Testing bad id in route

    status = room_service->get(in, out, "/state/not_an_id", owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);
    status = room_service->post(in, out, "/join/not_an_id", joiner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);

    room_service->get_room_start_infos(room_id);  // should throw since room is deleted
    room_service->set_room_state(room_id, server::Room_State::IN_GAME);
    // Testing room deletion when owner exits

    status = room_service->post(in, out, "/exit/" + std::to_string(room_id), owner_session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    player_manager.refresh_last_seen(owner_session_token);
    player_manager.refresh_last_seen(joiner_session_token);
}
