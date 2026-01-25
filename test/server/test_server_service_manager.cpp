#include <boost/test/unit_test.hpp>

#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Service_Manager)
{
    server::Player_Manager  player_manager;
    server::Service_Manager service_manager(player_manager);

    // Testing initialization - services vector should be empty
    BOOST_CHECK_EQUAL(service_manager.find_responsible_service("/unknown"), nullptr);

    // Create and register a Room_Service
    auto room_service = std::make_shared<server::Room_Service>(player_manager);
    service_manager.register_service(room_service);

    // Testing find_responsible_service
    server::Service_Interface* found_service = service_manager.find_responsible_service("/rooms");
    BOOST_CHECK(found_service != nullptr);
    BOOST_CHECK_EQUAL(found_service->get_root_prefix(), "/rooms");

    // Testing handle request

    std::string         in     = "player_name";
    std::string         out    = "";
    server::Http_Status status = service_manager.handle_request(in, out, "/connect", "POST", "");
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);
    std::string session_token = out;
    BOOST_CHECK(!session_token.empty());

    // get and post

    status = service_manager.handle_request(in, out, "/rooms/create", "POST", session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::CREATED);
    int room_id = std::stoi(out);

    status = service_manager.handle_request(in, out, "/rooms/state/" + std::to_string(room_id),
                                            "GET", session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::OK);

    // unauthorized
    status = service_manager.handle_request(in, out, "/rooms/create", "POST", "invalid_token");
    BOOST_CHECK_EQUAL(status, server::Http_Status::UNAUTHORIZED);

    // service not found
    status =
        service_manager.handle_request(in, out, "/unknown_service/action", "GET", session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::NOT_FOUND);

    // method not allowed in a service
    status = service_manager.handle_request(in, out, "/rooms/", "DELETE", session_token);
    BOOST_CHECK_EQUAL(status, server::Http_Status::METHOD_NOT_ALLOWED);
}
