#include <iostream>

#include "server.h"
namespace server
{

Room_Service::Room_Service(Server_Manager& server_manager)
    : Service_Interface("/rooms"), server_manager(server_manager)
{
}

Http_Status Room_Service::get(std::string& in, std::string& out, std::string url,
                              std::string session_token)
{
    if (url.find("/state/") != 0)
    {
        return Http_Status::BAD_REQUEST;
    }

    size_t      slash_pos   = url.find('/', 1);
    std::string action      = url.substr(0, slash_pos);
    std::string room_id_str = url.substr(slash_pos + 1);

    int room_id = std::stoi(room_id_str);
    std::cout << "Room_id : " << room_id << "Action :" << action << std::endl;
    if (action == "/state")
    {
        try
        {
            std::string room_state = server_manager.get_room_state(room_id, session_token);
            std::cout << "Room state: " << room_state << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << "Error while getting room state: " << e.what() << std::endl;
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::OK;
    }
}

Http_Status Room_Service::post(std::string& in, std::string& out, std::string url,
                               std::string session_token)
{
    // creating a room
    if (url == "/create")
    {
        try
        {
            int room_id = server_manager.create_room(session_token);
            out         = std::to_string(room_id);
        }
        catch (std::exception& e)
        {
            out = "Error while creating room: " + std::string(e.what());
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::CREATED;
    }

    if (url.find("/join/") != 0 && url.find("/exit/") != 0)
    {
        return Http_Status::BAD_REQUEST;
    }

    // action on a specific room -> /join/{id} or /exit/{id}

    size_t      slash_pos   = url.find('/', 1);
    std::string action      = url.substr(0, slash_pos);
    std::string room_id_str = url.substr(slash_pos + 1);
    int         room_id     = std::stoi(room_id_str);

    if (server_manager.verify_room_id(room_id) == false)
    {
        return Http_Status::NOT_FOUND;
    }
    std::cout << "room id : " << room_id_str << " action : " << action << std::endl;
    if (action == "/join")
    {
        try
        {
            server_manager.join_room(room_id, session_token);
        }
        catch (std::exception& e)
        {
            out = "Error while joining room: " + std::string(e.what());
            std::cout << "Error while joining room: " << e.what() << std::endl;
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::OK;
    }
    if (action == "/exit")
    {
        try
        {
            server_manager.exit_room(room_id, session_token);
        }
        catch (std::exception& e)
        {
            out = "Error while exiting room: " + std::string(e.what());
            std::cout << "Error while exiting room: " << e.what() << std::endl;
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::OK;
    }
    return Http_Status::METHOD_NOT_ALLOWED;
}
}  // namespace server