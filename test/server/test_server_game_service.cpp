#include <boost/test/unit_test.hpp>

#include "engine.h"
#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Game_Service)
{
    server::Player_Manager                player_manager;
    std::shared_ptr<server::Room_Service> room_service =
        std::make_shared<server::Room_Service>(player_manager);
    std::shared_ptr<server::Game_Service> game_service =
        std::make_shared<server::Game_Service>(room_service);
    room_service->set_ref_to_game_service(game_service);

    // Testing initialization
    BOOST_CHECK_EQUAL(game_service->get_root_prefix(), "/game");
    BOOST_CHECK_EQUAL(game_service->get_engines_thread_flag(), true);

    // Testing get_engines
    auto& engines = game_service->get_engines();
    BOOST_CHECK_EQUAL(engines.size(), 0);

    // Testing launch_game
    std::string creator_token = player_manager.create_player("Player1");
    int         room_id       = room_service->create_room(creator_token);

    std::vector<std::string>        player_names = {"Player1", "Player2", "Player3", "Player4"};
    std::vector<state::Player_Type> player_types = {
        state::Player_Type::Random_AI, state::Player_Type::Human, state::Player_Type::Heuristic_AI,
        state::Player_Type::Advanced_AI};

    game_service->launch_game(room_id, player_names, player_types);

    // Check that game was created
    BOOST_CHECK(engines.size() > 0);

    // Testing get methods
    std::string         in;
    std::string         out;
    server::Http_Status status =
        game_service->get(in, out, "/state/" + std::to_string(room_id), creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    status = game_service->get(in, out, "/version/" + std::to_string(room_id), creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    status = game_service->get(in, out, "/unknown_action", creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::METHOD_NOT_ALLOWED);

    // Testing post methods

    std::shared_ptr<engine::Choose_Species_Command> command =
        std::make_shared<engine::Choose_Species_Command>(0, 0);
    Json::Value command_json;
    command->to_json(command_json);
    Json::StreamWriterBuilder writer_builder;
    std::string               command_str = Json::writeString(writer_builder, command_json);

    in     = command_str;
    status = game_service->post(in, out, "/command/" + std::to_string(room_id), creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);

    status = game_service->post(in, out, "/command/not_an_id", creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);

    in     = "{";
    status = game_service->post(in, out, "/command/" + std::to_string(room_id), creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::BAD_REQUEST);  // command parsing error

    status = game_service->post(in, out, "/unknown_action", creator_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::METHOD_NOT_ALLOWED);

    // Testing stop_game
    game_service->stop_game(room_id);
    BOOST_CHECK_EQUAL(engines.size(), 0);

    // Starting another game with the "/start" route
}
